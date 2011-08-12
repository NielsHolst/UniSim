#include <usbase/exception.h>
#include <usbase/utilities.h>
#include "name_list.h"

namespace UniSim{

NameList::NameList(const QStringList &names_)
    : names(names_)
{
}

QStringList NameList::simplified() {
    if (names.isEmpty())
        return names;

    test = 0;
    prepareNames();
    root = new QObject;
    for (int i = 0; i < names.size(); ++i) {
        addNameToTree(names[i]);
    }

    names.clear();
    appendLeaves(root);
    delete root;
    return names;
}

void NameList::prepareNames() {
    names.sort();
    for (int i = 1; i < names.size(); ++i) {
        if (names[i-1] == names[i])
            throw Exception(QString("Name is not unique: %1").arg(names[i]));
    }
}

void NameList::addNameToTree(QString name) {
    QStringList items = name.split('/');
    QString tail;
    for (int i = items.size()-1; i >= 0; --i) {
        QString parent = tail;
        if (!tail.isEmpty())
            tail.prepend("/");
        tail.prepend(items[i]);
        addChildToTree(parent, tail);
    }
}

void NameList::addChildToTree(QString parentName, QString childName) {
    QObject *child = new QObject;
    child->setObjectName(childName);
    QObject *parent = parentName.isEmpty() ? root : root->findChild<QObject*>(parentName);
    Q_ASSERT(parent);
    child->setParent(parent);
    cout << qPrintable(QString("\n\n%1:%2 %3 =>\n").arg(++test).arg(parentName).arg(childName));
    writeObjectTree(root);
}

void NameList::appendLeaves(QObject *tree) {
    QObjectList children = tree->children();
    if (children.isEmpty())
        appendAtFork(tree);
    else for (int i = 0; i < children.size(); ++i) {
        appendLeaves(children[i]);
    }
}

void NameList::appendAtFork(QObject *tree) {
    QObject *base = tree;
    while (base->parent()->children().isEmpty()) {
        QObject *parent = base->parent();
        if (parent->parent())
            base = parent;
        else
            break;
    }
    names.append(base->objectName());
}




} //namespace
