#include <QObject>
#include <QVariant>
#include "exception.h"
#include "utilities.h"
#include "name_list.h"

namespace UniSim{

NameList::NameList(const QStringList &names_)
    : names(names_)
{
}

QStringList NameList::simplified() {
    if (names.isEmpty())
        return names;
    else if (names.size() == 1)
        return QStringList() << names[0].section("/",-1);

    root = new QObject;
    root->setObjectName("/");
    for (int i = 0; i < names.size(); ++i) {
        addNameToTree(names[i], i);
    }
    appendLeaves(root);

    result = names;
    for (int i = 0; i < leaves.size(); ++i) {
        putResult(leaves[i]);
    }

    delete root;
    return result;
}

void NameList::addNameToTree(QString name, int index) {
    QStringList items = name.split('/');
    QString tail;
    for (int i = items.size()-1; i >= 0; --i) {
        QString parent = tail;
        if (!tail.isEmpty())
            tail.prepend("/");
        tail.prepend(items[i]);
        addChildToTree(parent, tail, index);
    }
}

void NameList::addChildToTree(QString parentName, QString childName, int index) {
    bool childExists = root->findChild<QObject*>(childName) != 0;
    if (!childExists) {
        QObject *child = new QObject;
        child->setObjectName(childName);
        child->setProperty("index", index);
        QObject *parent = parentName.isEmpty() ? root : root->findChild<QObject*>(parentName);
        Q_ASSERT(parent);
        child->setParent(parent);
    }
}

void NameList::appendLeaves(QObject *object) {
    QObjectList children = object->children();
    if (children.isEmpty())
        leaves.append(object);
    else for (int i = 0; i < children.size(); ++i) {
        appendLeaves(children[i]);
    }
}

namespace {
    inline bool hasSiblings(QObject *object) {
        QObject *parent = object->parent();
        return parent ? parent->children().size() > 1 : true;
    }
}

void NameList::putResult(QObject *object) {
    QObject *p = object;
    while (!hasSiblings(p)) {
        p = p->parent();
    }
    bool ok;
    int index = p->property("index").toInt(&ok);
    if (ok && index < result.size())
        result[index] = p->objectName();
}




} //namespace
