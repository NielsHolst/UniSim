/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QXmlQuery>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include "xml_expander.h"
#include "xml_query_to_writer.h"


namespace {
	
	class XmlExpanderEx: public UniSim::Exception
	{
	public:
		XmlExpanderEx(QString fileName, QString message)
            : UniSim::Exception(QString())
		{
			QTextStream text(&_message);
			text << "Error when expanding UniSim file: "  <<  fileName << "\n"
				  << "Error message: " << message;
		}
	};

    const int MAX_LEVELS = 100;
}


namespace UniSim {

XmlExpander::XmlExpander(QString xmlFilePath_)
    : xmlFilePath(xmlFilePath_)
{
}

void XmlExpander::expand() {
	QFile f, g;	
	int i = 0;
	do {	
		f.close();
		g.close();
        QString inFilePath = (i == 0) ? xmlFilePath : temporaryFilePath(QString::number(i));
        QString outFilePath = temporaryFilePath(QString::number(i+1));
        f.setFileName(inFilePath);
        g.setFileName(outFilePath);
		       
        if (!f.open(QIODevice::Text | QIODevice::ReadOnly))
            throw XmlExpanderEx(xmlFilePath, "Cannot open file: " + inFilePath);
        if (!g.open(QIODevice::Text | QIODevice::WriteOnly))
            throw XmlExpanderEx(xmlFilePath, "Cannot open file: " + outFilePath);
    } while (i++ < MAX_LEVELS && expand(&f, &g));

    if (i == MAX_LEVELS)
        throw XmlExpanderEx(xmlFilePath, "XML file cannot be expanded more than " +
                            QString::number(MAX_LEVELS) + "times.");

    _newFileName = f.fileName();
} 

QString XmlExpander::temporaryFilePath(QString tail) const {
    QDir tempFolder = FileLocations::location(FileLocationInfo::Temporary);
    QString path = filePathWithTail(tail);
    QString name = QFileInfo(path).fileName();
    return tempFolder.path() + "/" + name;
}

QString XmlExpander::filePathWithTail(QString tail) const {
    QString path = xmlFilePath;
    int pos = path.indexOf(".");
    return (pos < 0) ? path + tail : path.insert(pos, tail);
}

bool XmlExpander::expand(QFile *fromXml, QFile *toXml) {
	bool expanded = false;
	
    reader = new QXmlStreamReader;
	reader->setDevice(fromXml);
    writer = new QXmlStreamWriter(toXml);

	writer->setAutoFormatting(true);
	writer->setCodec("ISO-8859-1");
	writer->writeStartDocument();
	
    bool writeEnd = true;
    while (!reader->isEndDocument()) {
        readNext();
        ElementType type = elementType();

        QString elementName;
        if (type==Begin || type==Select) {
            elementName = reader->name().toString();
            path.push(elementName);
        }

        switch (type) {
        case Begin:
            writer->writeStartElement(elementName);
            writer->writeAttributes(reader->attributes());
            break;
        case Select:
            writeQuery(reader->attributes().value("select").toString());
            writeEnd = false;
            expanded = true;
            break;
        case Other:
            break;
        case End:
            path.pop();
            if (writeEnd){
                writer->writeEndElement();
            }
            writeEnd = true;
        }

	}

	writer->writeEndDocument();
	delete reader;
	delete writer;
	
	return expanded;
}

void XmlExpander::readNext() {
    reader->readNext();
    checkReader();
}

void XmlExpander::checkReader() {
    if (reader->hasError()) {
        QString msg = QString("Error in UniSim input file.")
            + "\nIn line: " + QString::number(reader->lineNumber())
            + "\nIn column: " + QString::number(reader->columnNumber())
            + "\nXML-reader reported:" + reader->errorString()
            + "\nLast token read: " + reader->tokenString();
        throw XmlExpanderEx(xmlFilePath, msg);
    }
}

XmlExpander::ElementType XmlExpander::elementType() const {
    ElementType type;
    if (reader->isStartElement()) {
        if (reader->attributes().hasAttribute("select"))
            type = Select;
        else
            type = Begin;
    }
    else if (reader->isEndElement())
       type = End;
    else
       type = Other;
    return type;
}

void XmlExpander::writeQuery(QString queryString    )
{
    QXmlQuery query;
    QString queryStr = queryString.trimmed();
    if (queryStr.startsWith("doc")) {
        if (!queryStr.contains("http"))
            queryStr = insertDocPath(queryStr);
    }
    else
        queryStr = "doc('" + xmlFilePath + "')" + stackedPath() + "/" + queryStr;

    query.setQuery(queryStr);
    if (!query.isValid())
        throw XmlExpanderEx(xmlFilePath, "UniSim file contains illegal 'select' statement. "
                            "This is not legal XQuery syntax: " + queryStr);
    bool ok;

    /*
    XmlQueryToWriter toWriter(&query, writer);
    ok = query.evaluateTo(&toWriter);
    if (!ok)
        throw XmlExpanderEx(xmlFilePath, "Could not carry out XQuery: " + queryStr);
    */

    QString qResult;
    ok = query.evaluateTo(&qResult);
    if (!ok)
        throw XmlExpanderEx(xmlFilePath, "Could not carry out XQuery: " + queryStr);

    std::cout << "\n" << qPrintable(qResult) << "\n";

    QXmlStreamReader qReader(qResult);
	while (!qReader.atEnd()) {
        qReader.readNext();
		if (qReader.isStartElement()) {
            QString elementName = reader->name().toString();
            std::cout << qPrintable("START " + elementName + "\n");
            writer->writeStartElement(elementName);
            writer->writeAttributes(qReader.attributes());
		}
		else if (qReader.isEndElement()) {
            std::cout << "END\n";
            writer->writeEndElement();
		}
	}
    if (qReader.hasError()) {
        QString msg = "Error in 'select' XML.\n"
                      "Query = \"" + queryStr + "\"\n"
                      "Result = \"" + qResult + "\"\n";
		throw XmlExpanderEx(xmlFilePath, msg);
	}

}

QString XmlExpander::insertDocPath(QString queryStr) const {
    int from = queryStr.indexOf("('");
    int to = queryStr.indexOf("')");
    int length = to - from - 2;
    if (from==-1 || to==-1 || length<=0)
        throw XmlExpanderEx(xmlFilePath, "Name of document in XML 'select' not properly parenthesized: " +
                            queryStr + ". Use this format: doc('my_file.xml')/...");
    QString docName = queryStr.mid(from + 2, length);

    int tailFrom = queryStr.indexOf("/");
    QString tail = queryStr.mid(tailFrom);
    QString path = QFileInfo(xmlFilePath).absoluteDir().absolutePath();
    QString docPath = path + "/" + docName;

    QString result = "doc('" + docPath + "')" + tail;
    return result;
}


QString XmlExpander::stackedPath () const
{
	QString s;
    for (int i = 0; i < path.size(); ++i)
        s += "/" + path[i];
	return s;
}	

QString XmlExpander::newFileName() const {
    return _newFileName;
}

} // namespace
