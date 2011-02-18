/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QFile>
#include <QTextStream>
#include <QtXml/QXmlStreamReader>
#include "authors.h"
#include "exception.h"

namespace UniSim{

Authors* Authors::_authors = 0;

Authors::Authors()
    : reader(0)
{
    reader = new QXmlStreamReader;
}

Authors::~Authors() {
    delete reader;
}

QString Authors::id() {
    return "Authors";
}

Authors::Author Authors::find(Identifier id) {
    if (!theCollection.contains(id))
        throw Exception("Author id '" + id.label() + "' is missing");
    return theCollection.value(id);
}

const QMap<Identifier, Authors::Author>* Authors::collection() {
    return &theCollection;
}

void Authors::initialize() {
    QFile file(":/authors.xml");
    if (!file.open(QIODevice::ReadOnly))
        throw Exception("Cannot fint embedded file 'authors.xml");
    reader->setDevice(&file);

    nextToken();
    if (!reader->isStartElement() && elementNameEquals("authors"))
        throw Exception(message("Expected <authors> element'"));

    nextToken();
    while (!reader->hasError() && reader->isStartElement()) {
        if (elementNameEquals("author")) {
            readAuthor();
        } else {
            throw Exception(message("Unexpected element: '" + elementName() + "'"));
        }
        if(!reader->isEndElement())
            throw Exception(message("Expected end element </author>"));
        nextToken();
    }

    //nextToken();
    if(!reader->isEndElement())
        throw Exception(message("Expected end element </authors>"));
}

bool Authors::nextToken() {
    if (!reader->hasError() && !reader->isEndDocument()) {
        do {
            do {
                reader->readNext();
            } while (reader->isCharacters() && elementText().isEmpty());
        } while (!reader->hasError() && !reader->isEndDocument() &&
                !reader->isStartElement() && !reader->isEndElement() && !reader->isCharacters());
    }
    return reader->isStartElement() || reader->isEndElement() || reader->isCharacters();
}

bool Authors::elementNameEquals(QString s) const {
    return reader->name().compare(s, Qt::CaseInsensitive) == 0;
}

QString Authors::elementName() const {
    return reader->name().toString();
}

QString Authors::elementText() const {
    return reader->text().toString().trimmed();
}

void Authors::readAuthor() {
    Author author;
    nextToken();
    while (!reader->hasError() && reader->isStartElement()) {
        if (elementNameEquals("id")) {
            author.id = readText();
        }
        else if (elementNameEquals("name")) {
            author.name = readText();
        }
        else if (elementNameEquals("address")) {
            author.address = readText();
        }
        else if (elementNameEquals("email")) {
            author.email = readText();
        }
        else {
            throw Exception(message("Unexpected element: '" + elementName() + "'"));
        }
    }
    if (author.id.label().isEmpty())
        throw Exception(message("Missing author id"));
    if (author.name.isEmpty())
        throw Exception(message("Missing author name"));
    if (author.address.isEmpty())
        throw Exception(message("Missing author address"));
    if (author.email.isEmpty())
        throw Exception(message("Missing author email"));
    if (theCollection.contains(author.id))
        throw Exception(message("Author id occurs twice"));
    theCollection[author.id] = author;
}

QString Authors::readText() {
    nextToken();
    if (!reader->isCharacters())
        throw Exception(message("Text expected"));
    QString text = elementText();

    nextToken();
    if (!reader->isEndElement())
        throw Exception(message("End element expected"));

    nextToken();
    return text;
}

QString Authors::message(QString text) const {
    QString s;
    QTextStream str(&s);
    str   << "Error reading embedded file authors.xml\n"
          << "In line: " << reader->lineNumber() << "\n"
          << "In position: " << reader->columnNumber() << "\n"
          << (text.isEmpty() ? QString() : "Error message: " + text) << "\n"
          << "XML-reader reported: " << reader->errorString() << "\n"
          << "Last token read: " << reader->tokenString();
    return s;
}

} //namespace
