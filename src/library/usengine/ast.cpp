/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <QFile>
#include <usbase/const_mapped_file.h>
#include <usbase/exception.h>
#include "ast.h"

BOOST_FUSION_ADAPT_STRUCT(
        grammar::KeyValue,
        (std::string, key)
        (std::string, value)
        (std::string, type)
)
BOOST_FUSION_ADAPT_STRUCT(
        grammar::Output,
        (grammar::KeyValueList, reset)
        (grammar::KeyValueList, init)
)

BOOST_FUSION_ADAPT_STRUCT(
        grammar::ObjectDeclaration,
        (std::string, className)
        (std::string, objectName)
)

BOOST_FUSION_ADAPT_STRUCT(
        grammar::Model,
        (grammar::ObjectDeclaration, objectDeclaration)
        (grammar::KeyValueList, input)
        (grammar::Output, output)
        (grammar::ModelList, modelList)
)

namespace grammar {
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;

    typedef Model Result;

    template <typename Iterator>
    struct my_parser : qi::grammar<Iterator, Result(), ascii::space_type>
    {
        my_parser() : my_parser::base_type(model) {
            using ascii::char_;
            using phoenix::at_c;
            using phoenix::construct;
            using phoenix::val;
            using qi::eps;
            using qi::lexeme;
            using qi::lit;
            using namespace qi::labels;

            quoted_string %= lexeme['"' >> *(char_ - '"') >> '"'];
            unquoted_string %= lexeme[+(char_ - '-' - char_("\"=()[]{}<>, "))];
            reference %=
                       unquoted_string
                    >> ascii::string("[")
                    >> unquoted_string
                    >> ascii::string("]");
            value_ %= (-char_('-') >> unquoted_string) | quoted_string;
            identifier %= unquoted_string;

            key_value_1 =
                       ascii::string("->")      [at_c<2>(_val) = _1]
                    >> reference                [at_c<1>(_val) = _1];
            key_value_2 =
                       identifier               [at_c<0>(_val) = _1]
                    >> ascii::string("=")       [at_c<2>(_val) = _1]
                    >> value_                   [at_c<1>(_val) = _1];
            key_value_3 =
                       identifier               [at_c<0>(_val) = _1]
                    >> ascii::string("->")      [at_c<2>(_val) = _1]
                    >> reference                [at_c<1>(_val) = _1];
            key_value_4 =
                    value_                      [at_c<1>(_val) = _1];

            key_value %= key_value_1|key_value_2|key_value_3|key_value_4;

            parameter_list %= "(" >> -(key_value % ',') > ")";
            reset_list %= "[" >> -(key_value % ',') > "]";
            init_list %= "[[" >> -(key_value % ',') > "]]";
            model_list %=
                (   "{" >> *model >> "}" )
                |
                eps;
            output =
                (      init_list [at_c<1>(_val) = _1]
                    >> -reset_list [at_c<0>(_val) = _1]
                )
                |
                (      reset_list [at_c<0>(_val) = _1]
                    >> -init_list [at_c<1>(_val) = _1]
                )
                |
                eps;

            class_name %= unquoted_string;
            object_name %= unquoted_string;
            object_declaration =
                    class_name              [at_c<0>(_val) = _1]
                    >> -object_name         [at_c<1>(_val) = _1];
            model %= object_declaration
                    > parameter_list
                    > output
                    > model_list;

            identifier.name("identifier");
            value_.name("value");
            reference.name("reference");
            quoted_string.name("quoted label");
            unquoted_string.name("unquoted label");
            class_name.name("class name");
            object_name.name("object name");
            key_value.name("value or reference");
            key_value_1.name("->reference");
            key_value_2.name("key=value");
            key_value_3.name("->reference");
            key_value_4.name("value");
            parameter_list.name("list of parameters");
            model_list.name("list of models");
            reset_list.name("list of output variables to reset");
            init_list.name("list of output variables to initialize");
            model.name("model");

            qi::on_error<qi::fail>
            (
                model,
                std::cout
                << val("Error! Expecting ")
                << _4 // what failed?
                << val(" here: \"")
                << construct<std::string>(_3, _2) // iterators to error-pos, end
                << val("\"")
                << std::endl
            );
        }

        qi::rule<Iterator, std::string(), ascii::space_type>
                identifier, value_, reference,
                quoted_string, unquoted_string,
                class_name, object_name;
        qi::rule<Iterator, KeyValue(), ascii::space_type>
                key_value, key_value_1, key_value_2, key_value_3, key_value_4;
        qi::rule<Iterator, KeyValueList(), ascii::space_type>
                parameter_list, reset_list, init_list;
        qi::rule<Iterator, Output(), ascii::space_type>
                output;
        qi::rule<Iterator, ObjectDeclaration(), ascii::space_type>
                object_declaration;
        qi::rule<Iterator, Model(), ascii::space_type>
                model;
        qi::rule<Iterator, ModelList(), ascii::space_type>
                model_list;
    };

    bool operator==(const grammar::KeyValue &a, const grammar::KeyValue &b) {
        return
            a.key == b.key &&
            a.value == b.value &&
            a.type == b.type;
    }

    bool operator==(const grammar::ObjectDeclaration &a, const grammar::ObjectDeclaration &b) {
        return
            a.className == b.className &&
            a.objectName == b.objectName;
    }

    bool inOrder(const grammar::KeyValueList &list) {
        auto i = list.begin();
        auto end = list.end();
        int c1(0), c2(0), c3(0);
        while (i!=end && i->noKey()) { ++i; ++c1; }
        while (i!=end && !i->noKey()) { ++i; ++c2; }
        while (i!=end && i->noKey()) { ++i; ++c3; }
        return c3==0;
    }

    QStringList Model::syntaxCheck(QString namePath) {
        QString myPath =
                namePath
                + "/"
                + objectDeclaration.className.c_str()
                + "::"
                + objectDeclaration.objectName.c_str();
        QString msg = "Unnamed parameters only allowed in the beginning of the parameter list. "
                "Concerning %1 of model %2";
        QStringList errors;
        if (!inOrder(input)) errors << msg.arg("input (...)").arg(myPath);
        if (!inOrder(output.reset)) errors << msg.arg("output [...]").arg(myPath);
        if (!inOrder(output.init)) errors << msg.arg("output [[...]]").arg(myPath);
        int n = (int) modelList.size();
        for (int i = 0; i < n; ++i)
            errors << child(i)->syntaxCheck(myPath);
        return errors;
    }

}//namespace

namespace UniSim {

Ast::Ast()
{
}

grammar::Model* Ast::createModel(QFileInfo file) {
    using boost::spirit::ascii::space;
    typedef const char* Iterator;
    typedef grammar::my_parser<Iterator> Parser;

    ConstMappedFile buffer;
    try {
        buffer.open(file.absoluteFilePath().toStdString());
    }
    catch (...) {
        QString msg("Could not open file for parsing '%1'");
        throw Exception(msg.arg(file.absoluteFilePath()));
    }
    Iterator first = buffer.begin();
    Iterator end = buffer.end();

    Parser parser;

    auto model = new grammar::Model;
    bool ok = phrase_parse(first, end, parser, space, *model);
    if (!ok) delete model;
    return ok ? model : 0;
}

grammar::Model* Ast::createModel(QString s) {
    using boost::spirit::ascii::space;
    typedef std::string::const_iterator Iterator;
    typedef grammar::my_parser<Iterator> Parser;

    const std::string &str{s.toStdString()};
    Iterator first = str.begin();
    Iterator end = str.end();
    Parser parser;

    auto model = new grammar::Model;
    bool ok = phrase_parse(first, end, parser, space, *model);
    if (!ok) delete model;
    return ok ? model : 0;
}

} // namespace
