/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_XML_EXPANDER_H
#define UniSim_XML_EXPANDER_H

#include <QStack>
#include <QString>

class QFile;
class QXmlStreamReader;
class QXmlStreamWriter;

namespace UniSim {

class XmlExpander
{
public: 
    XmlExpander(QString xmlFilePath);
	
	void expand(); 
	QString newFileName() const;
	
private:
    typedef enum {Begin, End, Select, Other} ElementType;
    QXmlStreamReader *reader;
    QXmlStreamWriter *writer;

    QString temporaryFilePath(QString tail) const;
    QString finalFilePath() const;
    QString filePathWithTail(QString tail) const;
    QString insertDocPath(QString queryStr) const;

	bool expand(QFile *fromXml, QFile *toXml);
    void readNext();
    ElementType elementType() const;
    void checkReader();

	QString stackedPath () const;
    void writeQuery(QString queryString);

    QString xmlFilePath, _newFileName;
    QStack<QString> path;

};

} //namespace

#endif

