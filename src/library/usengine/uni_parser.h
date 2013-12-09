/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_UNI_PARSER_H
#define UNISIM_UNI_PARSER_H

#include <locale>
#include <QIODevice>
#include <QString>

namespace UniSim {

class UniParser {
public:
    UniParser();
    UniParser(QString filePath);
    ~UniParser();
    void parse(QString outputFilePath);
    void parse(QString input, QStringList &texts);

private:
    QIODevice *input;
    char nextChar;
    bool betweenTextDelimiters, passedEnd;
    struct Token {
        QString text;
        enum {Text, Colon, DoubleColon, Asterisc, LeftBrace, RightBrace, End} type;
    };
    Token token;
    QList<Token> tokens;

    void doParse();
    void readNextChar();
    char getChar();
    void readNextToken();
    void readNextSignificantChar();
    bool symbolChar() const;
    bool wsChar() const;

    void secondParse();
    void simulation();
    void parameter();
    void controller();
    void model();
    void outputs();
    void plot();
    void trace();
};

inline bool UniParser::symbolChar() const {
    return QString(":*{}").contains(nextChar);
}

inline bool UniParser::wsChar() const {
    return std::isspace(nextChar);
}


}

#endif
