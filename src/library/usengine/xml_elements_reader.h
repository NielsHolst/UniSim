/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_XML_ELEMENTS_READER_H
#define UNISIM_XML_ELEMENTS_READER_H
#include <QtXml/QXmlStreamReader>

class QFile;

namespace UniSim {

class XmlElementsReader {
public:
    XmlElementsReader();
    ~XmlElementsReader();
    void open(QString filePath);
    void next();
    bool isStartTag() const;
    bool isEndTag() const;
    bool isEndDocument() const;
    QXmlStreamAttributes attributes() const;
    QString name() const;
    void hurl(QString message);
private:
    QString filePath;
    QXmlStreamReader *reader;
    QFile *file;
};


} //namespace

#endif
