/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QBuffer>
#include <QFile>
#include <QStringList>
#include <usbase/exception.h>
#include "uni_parser.h"

namespace UniSim {

UniParser::UniParser()
    : input(0), betweenTextDelimiters(false)
{
}

UniParser::UniParser(QString filePath)
    : betweenTextDelimiters(false)
{
    input = new QFile(filePath);
    if (!input->open(QIODevice::ReadOnly)) {
        QString msg("Cannot open file '%1' for reading.");
        throw Exception(msg.arg(filePath));
    }
}

UniParser::~UniParser() {
    delete input;
}

void UniParser::parse(QString outputFilePath) {
    doParse();
    QFile output(outputFilePath);
    if (!output.open(QIODevice::WriteOnly)) {
        QString msg("Cannot open file '%1' for writing.");
        throw Exception(msg.arg(outputFilePath));
    }
    for (int i = 0; i < tokens.size(); ++i) {
        QString s1 = QString::number(int(tokens.at(i).type));
        QString s2 = tokens.at(i).text;
        output.write(qPrintable(s1+" "+s2+"\n"));
    }
}

void UniParser::parse(QString inputText, QStringList &texts) {
    input = new QBuffer;
    input->open(QBuffer::ReadWrite);
    input->write(qPrintable(inputText));
    input->seek(0);
    doParse();

    texts.clear();
    for (int i = 0; i < tokens.size(); ++i)
        texts << tokens.at(i).text;
}

void UniParser::doParse() {
    Q_ASSERT(input);
    tokens.clear();
    readNextSignificantChar();
    while (!passedEnd) {
        readNextToken();
        tokens << token;
    }
    if (betweenTextDelimiters)
        throw Exception("Missing closing apostrophe");
    input->close();
    secondParse();
}

void UniParser::readNextToken() {
    token.text.clear();
    while (!passedEnd) {
        if (symbolChar() && !betweenTextDelimiters) {
            if (token.text.isEmpty()) {
                token.text = nextChar;
                readNextSignificantChar();
            }
            break;
        }
        else if (wsChar() && !betweenTextDelimiters) {
            readNextSignificantChar();
            break;
        }
        else {
            token.text += nextChar;
            readNextChar();
        }
    }
    if (token.text == ":") {
        if (nextChar == ':') {
            token.type = Token::DoubleColon;
            readNextChar();
        }
        else
            token.type = Token::Colon;
    }
    else if (token.text == "*")
        token.type = Token::Asterisc;
    else if (token.text == "{")
        token.type = Token::LeftBrace;
    else if (token.text == "}")
        token.type = Token::RightBrace;
    else if (!token.text.isEmpty()) {
        token.type = Token::Text;
        char firstChar = token.text[0].toLatin1();
        if (firstChar == 0)
            token.text.clear();
    }
    else if (passedEnd)
        token.type = Token::End;
    else
        throw Exception("Expected end of file");
}

void UniParser::readNextSignificantChar() {
    do {
        readNextChar();
    } while (wsChar() && !betweenTextDelimiters && !passedEnd);
}

void UniParser::readNextChar() {
    passedEnd = input->atEnd();
    if (passedEnd) {
        nextChar = 0;
    }
    else {
        int iter = 0;
        do {
            ++iter;
            nextChar = getChar();
            if (nextChar == '"')
                betweenTextDelimiters = !betweenTextDelimiters;
        } while (betweenTextDelimiters && nextChar=='"');
        if (nextChar == '"') {
            bool emptyText = (iter==2);
            if (emptyText)
                nextChar = 0;
            else {
                readNextChar();
                if (!passedEnd && !symbolChar() && !wsChar()) {
                    throw Exception("Apostrophe collides with following text");
                }
            }
        }
    }
}

char UniParser::getChar() {
    Q_ASSERT(!input->atEnd());
    char ch;
    bool ok = input->getChar(&ch);
    if (!ok) {
        QString msg("Cannot read from uni file.\n");
        throw Exception(msg + input->errorString());
    }
    return ch;
}

void UniParser::secondParse() {
}

void UniParser::simulation() {

}

void UniParser::parameter() {

}

void UniParser::controller() {

}

void UniParser::model() {

}

void UniParser::outputs() {

}

void UniParser::plot() {

}

void UniParser::trace() {

}

} // namespace
