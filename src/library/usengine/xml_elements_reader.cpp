/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QtXml/QXmlStreamReader>
#include <usbase/exception.h>
#include "xml_elements_reader.h"

namespace {
	
	class UniSimEx : public UniSim::Exception
	{
	public:
		UniSimEx(QString fileName, QXmlStreamReader* reader, QString message = QString())
            : UniSim::Exception(QString())
		{
			QTextStream text(&_message);
            text << "Error reading prototype file: "  <<  fileName << "\n"
				  << "In line: " << reader->lineNumber() << "\n"
				  << "In position: " << reader->columnNumber() << "\n"
				  << (message.isEmpty() ? QString() : "Error message: " + message) << "\n"
				  << "XML-reader reported: " << reader->errorString() << "\n"
				  << "Last token read: " << reader->tokenString();
		}
	};
	
}


namespace UniSim{
	
XmlElementsReader::XmlElementsReader() {
    reader = new QXmlStreamReader;
    file = new QFile;
}

XmlElementsReader::~XmlElementsReader() {
	delete reader;
    delete file;
}

void XmlElementsReader::open(QString filePath_) {
    filePath = filePath_;

    file->setFileName(filePath);
    if (!file->open(QIODevice::ReadOnly))
        hurl("Cannot open file: '"+filePath+"' for reading.");
    reader->setDevice(file);
}

void XmlElementsReader::next() {
    if (!reader->hasError() && !reader->isEndDocument()) {
		do {
            reader->readNext();
        } while (!reader->hasError() && !reader->isEndDocument() &&
				!reader->isStartElement() && !reader->isEndElement()); 
	}
    if (reader->hasError()) hurl("Illegal XML syntax");
}

bool XmlElementsReader::isStartTag() const {
    return reader->isStartElement();
}

bool XmlElementsReader::isEndTag() const {
    return reader->isEndElement();
}

bool XmlElementsReader::isEndDocument() const {
    return reader->isEndDocument();
}

QXmlStreamAttributes XmlElementsReader::attributes() const {
    return reader->attributes();
}

QString XmlElementsReader::name() const {
    return reader->name().toString();
}

void XmlElementsReader::hurl(QString message) {
    throw UniSimEx(filePath, reader, message);
}

} //namespace
