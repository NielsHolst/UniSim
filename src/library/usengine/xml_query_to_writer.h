/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XML_QUERY_TO_WRITER_H
#define XML_QUERY_TO_WRITER_H
#include <QAbstractXmlReceiver>
 
class QXmlQuery;
class QXmlStreamWriter;
 
namespace UniSim {

//! \addtogroup UniSim
//@{
//! Class %XmlQueryToWriter resolves an XQuery reference from one UniSim file to another
/*!
	The class is used by XmlExpander to expand an XQuery reference by copying the result of the query
	into a QXmlStreamWriter stream. Only elements and attributes are copied. See also the QAbstractXmlReceiver
	interface class which %XmlQueryToWriter is an implementation of.
	
	The code of %XmlQueryToWriter is short and simple.
*/
class XmlQueryToWriter : public QAbstractXmlReceiver
{
public:
	//! Constructor
	/*!
		\param query object containing the XQuery string
		\param writer stream that the result of the query will be copied to
	*/
	XmlQueryToWriter(QXmlQuery *query, QXmlStreamWriter *writer); 
	
	//! Ignores atomic values
	void atomicValue ( const QVariant & value ) { }
	
	//! Copies attribute
	void attribute ( const QXmlName & name, const QStringRef & value);
	
	//! Ignores characters
	void characters ( const QStringRef & value ) { }
	
	//! Ignores comments
	void comment ( const QString & value ) { }
	
	//! Ignores end of document
	void endDocument () { }
	
	//! Copies end of element
	void endElement ();
	
	//! Ignores end of sequence
	void endOfSequence () { }
	
	//! Ignores namespace
	void namespaceBinding ( const QXmlName & name ) { }
	
	//! Ignores instruction
	void processingInstruction ( const QXmlName & target, const QString & value ) { }
	
	//! Ignores start of document
	void startDocument () { }
	
	//! Copies start of element
	void startElement ( const QXmlName & name );
	
	//! Ignores start of sequence
	void startOfSequence () { }
private:
	QXmlQuery *_query;
	QXmlStreamWriter *_writer;
};
//@}
} //namespace

#endif
