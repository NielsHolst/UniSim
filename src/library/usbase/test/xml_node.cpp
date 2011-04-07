/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QList>
#include <QXmlStreamReader>
#include <usbase/exception.h>
#include "xml_node.h"


namespace UniSim{

XmlNode::XmlNode(Identifier name, QObject *parent)
    : NamedObject(name, parent)
{
    setAttribute("name", "anonymous");
}

XmlNode* XmlNode::fromString(QString s_) {
    QString s;
    s = "<root>" + s_ + "</root>";

    QXmlStreamReader reader(s);
    reader.readNext();
    Q_ASSERT(reader.isStartDocument());

    reader.readNext();

    XmlNode *root = new XmlNode("root", 0);
    root->readNode(&reader, 0);

    if (reader.hasError() || !reader.isEndDocument()) {
        QString msg = "Error converting XML:\n\"" + s_ +"\"\n";
        throw Exception(msg);
    }

    return root;
}

void XmlNode::readNode(QXmlStreamReader *reader, QObject *parent) {
    while (!reader->hasError() && !reader->atEnd() && reader->isStartElement()) {
        QString name = reader->name().toString();
        XmlNode *node = parent ? new XmlNode(name, parent) : this;

        QXmlStreamAttributes attr = reader->attributes();
        for (int i = 0; i < attr.size(); ++i)
            node->setAttribute(attr[i].name().toString(), attr[i].value().toString());

        reader->readNext();
        if (reader->isStartElement())
            readNode(reader, node);
        else if (reader->isEndElement())
            reader->readNext();
    }

    if (reader->isEndElement())
        reader->readNext();
}

QString XmlNode::attribute(Identifier name) const {
    return _attributes.contains(name) ? _attributes.value(name) : QString();
}

void XmlNode::setAttribute(Identifier name, QString value) {
    _attributes[name] = value;
}

const XmlNode::Attributes& XmlNode::attributes() const{
    return _attributes;
}

XmlNode* XmlNode::childNode(int i) const {
    return dynamic_cast<XmlNode*>(children()[i]);
}

bool XmlNode::equals(const XmlNode *node) {
  bool equalId = id() == node->id();
  bool equalNameAttr = attribute("name") == node->attribute("name");
  return equalId && equalNameAttr;
}

void XmlNode::merge(XmlNode *node) {
    mergeAttributes(node);
    QList<XmlNode*> toAppend;
    int n = node->children().size();
    for (int i = 0; i < n; ++i) {
        XmlNode *mergeChild = node->childNode(i);
        XmlNode *myChild = findMatchingChild(mergeChild);
        if (myChild)
            myChild->merge(mergeChild);
        else
            toAppend.append(mergeChild);
    }
    for (int i = 0; i < toAppend.size(); ++i) {
        toAppend[i]->setParent(this);
    }
}

void XmlNode::mergeAttributes(XmlNode *node) {
    QMapIterator<Identifier, QString> at(node->attributes());
    while(at.hasNext()) {
        at.next();
        setAttribute(at.key(), at.value());
    }
}

XmlNode* XmlNode::findMatchingChild(const XmlNode *node) {
    // Find children with same node id, e.g. "model"
    QList<XmlNode*> myChildren = seekChildren<XmlNode*>(node->id().key());
    // Find child with same value for "name" attribute
    QString findName = node->attribute("name");
    XmlNode *foundChild = 0;
    for (int i = 0; i < myChildren.size(); ++i) {
        if (myChildren[i]->attribute("name") == findName) {
            if (foundChild)
                throw Exception(fullName() + " has two children with same name: " + findName);
            else
                foundChild = myChildren[i];
        }
    }
    return foundChild;
}

bool deepEquals(const XmlNode *a, const XmlNode *b) {
    if (a->attributes() != b->attributes())
        return false;
    int na = a->children().size();
    if (na != b->children().size())
        return false;
    for (int i = 0; i < na; ++i)
        if (!deepEquals(a->childNode(i), b->childNode(i)))
            return false;
    return true;
}

} //namespace
