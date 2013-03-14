/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QFile>
#include <QFileInfo>
#include <QList>
#include <QMessageBox>
#include <QXmlQuery>
#include <QXmlStreamReader>
#include <usbase/exception.h>
#include "xml_node.h"


namespace UniSim{

XmlNode::XmlNode(Identifier name, QObject *parent)
    : NamedObject(name, parent)
{
}

XmlNode* XmlNode::createFromString(QString s_) {
    if (s_.isEmpty())
        throw Exception("Cannot create XML node from empty string");

    QString s;
    s = "<root>" + s_ + "</root>";

    QXmlStreamReader reader(s);
    XmlNode *root = createFromReader(&reader, "from string: " + s);

    // Only keep root node if more than one child
    XmlNode *result = root;
    if (root->children().size() == 1) {
        result = root->childNode(0);
        result->setParent(0);
        delete root;
    }
    return result;
}

XmlNode* XmlNode::createFromFile(QString filePath) {
    QFile f(filePath);
    if (!f.open(QIODevice::Text | QIODevice::ReadOnly))
        throw Exception("Cannot open XML file: " + filePath);

    QXmlStreamReader reader(&f);
    return createFromReader(&reader, "from file: " + filePath);
}

XmlNode* XmlNode::createFromReader(QXmlStreamReader *reader, QString errorText) {
    while (!reader->hasError() && !reader->atEnd() && !reader->isStartElement())
        readNext(reader);
    QString name = reader->name().toString();

    XmlNode *root = 0;
    bool ok = reader->isStartElement();
    if (ok) {
        root = new XmlNode(name, 0);
        root->readNode(reader, 0);
    }

    ok = ok && !reader->hasError() && reader->isEndDocument();
    if (!ok) {
        QString msg = "Error converting XML " + errorText;
        throw Exception(msg);
    }

    Q_ASSERT(root);
    return root;
}

void XmlNode::readNode(QXmlStreamReader *reader, QObject *parent) {
    while (!reader->hasError() && !reader->atEnd() && reader->isStartElement()) {
        QString name = reader->name().toString();
        XmlNode *node = parent ? new XmlNode(name, parent) : this;

        QXmlStreamAttributes attr = reader->attributes();
        for (int i = 0; i < attr.size(); ++i)
            node->setAttribute(attr[i].name().toString(), attr[i].value().toString());

        readNext(reader);
        if (reader->isStartElement())
            readNode(reader, node);
        else if (reader->isEndElement())
            readNext(reader);
    }

    if (reader->isEndElement())
        readNext(reader);
}

void XmlNode::readNext(QXmlStreamReader *reader) {
    do {
        reader->readNext();
    }
    while (!reader->hasError() &&
           !reader->atEnd() &&
           !reader->isStartElement() &&
           !reader->isEndElement() &&
           !reader->isEndDocument());
}

void XmlNode::writeToFile(QString filePath, QString isoCode) const {
    QString header = "<?xml version=\"1.0\" encoding=\"" + isoCode + "\"?>\n";
    QString tree;
    const_cast<XmlNode*>(this)->getTree(&tree);

    QFile file(filePath);
    if (!file.open(QIODevice::Text | QIODevice::WriteOnly))
        throw Exception("Cannot open XML file for writing: " + filePath);
    file.write(qPrintable(header + tree));
}

void XmlNode::compile(QString filePath) {
    if (!QFileInfo(filePath).exists())
        throw Exception("File does not exist: " + filePath);
    QString queryAttr = attribute("select").trimmed();
    if (!queryAttr.isEmpty()) {
        QString queryInput = buildQueryString(filePath, queryAttr);
        QXmlQuery query;
        query.setQuery(queryInput);
        QString queryOutput;

        bool ok = query.evaluateTo(&queryOutput);
        queryOutput = queryOutput.trimmed();
        if (!ok)
            throw Exception("Could not carry out XQuery: " + queryInput);
        if (queryOutput.isEmpty())
            throw Exception("Nothing found that matches XQuery: " + queryInput);

        XmlNode *baseNode = XmlNode::createFromString(queryOutput);
        baseNode->setId(id());
        baseNode->merge(this);
        overtake(baseNode);
        _attributes.remove("select");
    }
    for (int i = 0; i < children().size(); ++i)
        childNode(i)->compile(filePath);
}

QString XmlNode::buildQueryString(QString filePath, QString queryAttr) const {
    QString query;;
    if (queryAttr.startsWith("doc")) {
        query = queryAttr.contains("http") ? queryAttr : insertDocPath(filePath, queryAttr);
    }
    else {
        query = "doc('file:///" + filePath + "')";
        if (!queryAttr.startsWith("/"))
            query += xmlPath() + "/";
        query += queryAttr;
    }
    return query;
}

QString XmlNode::insertDocPath(QString filePath, QString queryAttr) {
    int from = queryAttr.indexOf("('");
    int to = queryAttr.indexOf("')");
    int length = to - from - 2;
    if (from==-1 || to==-1 || length<=0)
        throw Exception("Name of document in XML 'select' not properly parenthesized: " +
                        queryAttr + " in " + filePath +
                        ". Use this format: doc('my_file.xml')/...");
    QString docName = queryAttr.mid(from + 2, length);

    int tailFrom = queryAttr.indexOf("/");
    QString tail = queryAttr.mid(tailFrom);
    QString path = QFileInfo(filePath).absoluteDir().absolutePath();
    QString docPath = path + "/" + docName;

    QString result = "doc('file:///" + docPath + "')" + tail;
    return result;
}

QString XmlNode::xmlPath() const {
    QString ancestors;
    if (parent())
        ancestors = dynamic_cast<XmlNode*>(parent())->xmlPath();
    QString selector;
    if (_attributes.contains("name"))
        selector = "[@name='" + attribute("name") + "']";
    return ancestors + "/" + id().label() + selector;
}

void XmlNode::overtake(XmlNode *victim) {
    // Delete my current children
    QObject *takeChildren = new QObject;
    for (int i = 0; i < children().size(); ++i)
        children()[i]->setParent(takeChildren);
    delete takeChildren;
    // Copy attribute and take over children from victim
    _attributes = victim->attributes();
    QList<QObject*> victimChildren = victim->children();
    for (int i = 0; i < victimChildren.size(); ++i)
        victimChildren[i]->setParent(this);
    delete victim;
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
    if (a->id() != b->id() || a->attributes() != b->attributes())
        return false;
    int na = a->children().size();
    if (na != b->children().size())
        return false;
    for (int i = 0; i < na; ++i)
        if (!deepEquals(a->childNode(i), b->childNode(i)))
            return false;
    return true;
}

void XmlNode::getTree(QString *s, int level) {
    if (level == 0)
        s->clear();
    QString margin;
    for (int i = 0; i < 2*level; ++i)
        margin += " ";
    *s +=  margin + "<" + id().label();
    for (int i = 0; i < _attributes.size(); ++i)
        *s += " " + _attributes.keys()[i].label() + "=\"" + _attributes.values()[i] + "\"";

    if (children().isEmpty()) {
        *s += "/>\n";
    }
    else {
        *s += ">\n";
        for (int i = 0; i < children().size(); ++i)
            childNode(i)->getTree(s,level+1);
        *s += margin + "</" + id().label() + ">\n";
    }
}

} //namespace
