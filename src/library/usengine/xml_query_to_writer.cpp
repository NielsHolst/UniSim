/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QXmlNamePool>
#include <QXmlQuery>
#include <QXmlStreamWriter>
#include "xml_query_to_writer.h"

namespace UniSim {

XmlQueryToWriter::XmlQueryToWriter(QXmlQuery *query, QXmlStreamWriter *writer)
	: _query(query), _writer(writer)
{
}

void XmlQueryToWriter::startElement(const QXmlName & name)
{
	_writer->writeStartElement(name.localName(_query->namePool()));
}
	
void XmlQueryToWriter::endElement()
{
	_writer->writeEndElement();
}

void XmlQueryToWriter::attribute(const QXmlName & name, const QStringRef & value)
{
	_writer->writeAttribute(name.localName(_query->namePool()), value.toString());
}

} //namespace

