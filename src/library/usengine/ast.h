/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_ENGINE_AST_H
#define UNISIM_ENGINE_AST_H
#include <string>
#include <vector>
#include <boost/variant.hpp> //includes recursive_wrapper<>
#include <QFileInfo>
#include <QString>
#include <QStringList>

#include <iostream>
class QFile;

namespace grammar {

    struct KeyValue {
        std::string key, value, type;

        KeyValue() : key(), value(), type("=") {}
        KeyValue(std::string key_, std::string value_, std::string type_)
            : key(key_), value(value_), type(type_) {}
        bool isReference() const { return type=="->"; }
        bool noKey() const { return key.size()==0 || key==value; }
    };
    typedef std::vector<KeyValue> KeyValueList;

    struct Output {
        KeyValueList reset, init;
    };

    struct Model;
    typedef boost::recursive_wrapper<Model> WrappedModel;
    typedef std::vector<WrappedModel> ModelList;

    struct ObjectDeclaration {
        std::string className, objectName;

        ObjectDeclaration() {}
        ObjectDeclaration(std::string className_, std::string objectName_)
            : className(className_), objectName(objectName_) {}
    };

    struct Model {
        ObjectDeclaration objectDeclaration;
        KeyValueList input;
        Output output;
        ModelList modelList;
        Model* child(int i) { Q_ASSERT(i < (int) modelList.size()); return modelList[i].get_pointer(); }
        QStringList syntaxCheck(QString namePath = "");
    };

    bool operator==(const KeyValue &a, const KeyValue &b);
    bool operator==(const ObjectDeclaration &a, const ObjectDeclaration &b);

} //namespace


namespace UniSim {

class Ast {
public:
    Ast();
    grammar::Model* createModel(QFileInfo file);
    grammar::Model* createModel(QString s);
};



}

#endif
