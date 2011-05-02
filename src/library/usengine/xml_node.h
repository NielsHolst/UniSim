/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_XML_NODE_H
#define UNISIM_XML_NODE_H

#include <QList>
#include <QMap>
#include <QObject>
#include <usbase/named_object.h>

class QXmlStreamReader;

namespace UniSim{

class XmlNode : public NamedObject
{
	Q_OBJECT
public:
    typedef QMap<Identifier, QString> Attributes;

    XmlNode(Identifier name, QObject *parent);
    static XmlNode* createFromString(QString s);
    static XmlNode* createFromFile(QString filePath);
    static XmlNode* createFromReader(QXmlStreamReader *reader, QString errorText);
    void writeToFile(QString filePath, QString isoCode="ISO-8859-1") const;
	
    void setAttribute(Identifier name, QString value);
    QString attribute(Identifier name) const;
    const Attributes& attributes() const;
    XmlNode* childNode(int i) const;
    QString xmlPath() const;
    void getTree(QString *s, int level = 0);

    void merge(XmlNode *node);
    void compile(QString filePath);
    QString buildQueryString(QString filePath, QString queryAttr) const;
    static QString insertDocPath(QString filePath, QString queryAttr);

private:
    void readNode(QXmlStreamReader *reader, QObject *parent);
    void mergeAttributes(XmlNode *node);
    XmlNode* findMatchingChild(const XmlNode *node);
    static void readNext(QXmlStreamReader *reader);
    void overtake(XmlNode *victim);

    Attributes _attributes;
};

bool deepEquals(const XmlNode *a, const XmlNode *b);


} //namespace


#endif
