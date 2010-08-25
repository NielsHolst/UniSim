/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
	
}


namespace UniSim {

XmlExpander::XmlExpander(QString xmlFile, QString appendedName)
	: _xmlFile(xmlFile), _appendedName(appendedName)
{
	if (xmlFile.size() == 0) throw XmlExpanderEx(xmlFile, "Name of XML file is empty");
	if (!QFile(xmlFile).exists()) throw XmlExpanderEx(xmlFile, "File not found");
	Q_ASSERT_X(QDir::isAbsolutePath(xmlFile), 
	                         "XmlExpander constructor", 
	                         qPrintable("Must have absolute file path, got " + xmlFile));
}

void XmlExpander::expand()
{
    int pos = _xmlFile.lastIndexOf(".");
	if (pos < 0) pos = _xmlFile.size();

	
	QFile f, g;	
	int i = 0;
	do {	
		f.close();
		g.close();
		f.setFileName(appendedFileName(i==0 ? QString() : "_0" + QString::number(i)));
		g.setFileName(appendedFileName("_0" + QString::number(i+1)));
		       
		if (!f.open(QIODevice::Text | QIODevice::ReadOnly))   throw XmlExpanderEx(_xmlFile, "Cannot open file: "+f.fileName());
		if (!g.open(QIODevice::Text | QIODevice::WriteOnly))  throw XmlExpanderEx(_xmlFile, "Cannot open file: "+g.fileName());
	} while (i++ < 20 && expand(&f, &g));

	f.close();
	g.close();
	if (i==20) throw XmlExpanderEx(_xmlFile, "XML file cannot be expanded more than 20 times. Maybe UniSim 'select' statements form an endless loop?");

	f.setFileName(newFileName());
	if (f.exists()) {
		if(!f.remove()) throw XmlExpanderEx(_xmlFile, "Cannot delete old " + newFileName());
	}
	
	if (!g.rename(newFileName()))
		throw XmlExpanderEx(_xmlFile, "Cannot rename " + g.fileName() + " to " + newFileName());
} 

QString XmlExpander::newFileName() const
{
    QString filePath = appendedFileName(_appendedName);
    QString name = QFileInfo(filePath).fileName();

    QDir tempFolder = FileLocations::location(FileLocations::Temporary);
    return tempFolder.path() + "/" + name;
}

QString XmlExpander::appendedFileName(QString append) const
{
	int pos = _xmlFile.indexOf(".");
	if (pos < 0) pos = _xmlFile.size();
	QString fn(_xmlFile);		
	return fn.insert(pos, append);
}
	
bool XmlExpander::expand(QFile *fromXml, QFile *toXml)
{
	bool expanded = false;
	
	QXmlStreamReader *reader = new QXmlStreamReader;
	reader->setDevice(fromXml);
	QXmlStreamWriter *writer = new QXmlStreamWriter(toXml);

	writer->setAutoFormatting(true);
	writer->setCodec("ISO-8859-1");
	writer->writeStartDocument();
	
	while (!reader->isEndDocument()) {
		reader->readNext();
		if (reader->hasError()) {
			QString msg = QString("Error in UniSim input file.") 
				+ "\nIn line: " + QString::number(reader->lineNumber())
				+ "\nIn column: " + QString::number(reader->columnNumber())
				+ "\nXML-reader reported:" + reader->errorString() 
				+ "\nLast token read: " + reader->tokenString();
			throw XmlExpanderEx(_xmlFile, msg);
		}
		if (reader->isStartElement()) {
			writer->writeStartElement(reader->name().toString());
			_path.push(reader->name().toString());
			for (int i = 0; i < reader->attributes().size(); ++i) {
				if (reader->attributes()[i].name() == "name") 
					_path.push(_path.pop() +"[@name='" + reader->attributes()[i].value().toString() + "']");
				if (reader->attributes()[i].name() != "select")  {
					writer->writeAttribute(reader->attributes()[i]);
				}
				else {
					writer->writeAttribute("copied-from", reader->attributes()[i].value().toString());
					writeQuery(stackedPath() + "/" + reader->attributes()[i].value().toString(), writer);
					expanded = true;
				}
			}
		}
		else if (reader->isEndElement()){
			writer->writeEndElement();
			_path.pop();
		}
	}

	writer->writeEndDocument();
	delete reader;
	delete writer;
	
	return expanded;
}

void XmlExpander::writeQuery(QString queryString, QXmlStreamWriter *writer)
{
	QXmlQuery query;
	QString queryStringWithDoc = "doc('" + _xmlFile + "')" + queryString;
	query.setQuery(queryStringWithDoc);
	if (!query.isValid()) throw XmlExpanderEx(_xmlFile, "UniSim file contains illegal 'select' statement. This not legal XQuery syntax: "+queryString);
	
	XmlQueryToWriter toWriter(&query, writer);
	bool ok = query.evaluateTo(&toWriter);
	if (!ok) throw XmlExpanderEx(_xmlFile, "Could not carry out XQuery: " + queryStringWithDoc);
			
}

QString XmlExpander::stackedPath () const
{
	QString s;
	for (int i = 0; i < _path.size(); ++i) 
		s += "/" + _path[i];
	return s;
}	

} // namespace
