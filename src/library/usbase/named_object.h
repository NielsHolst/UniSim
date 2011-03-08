/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_NAMED_OBJECT
#define UNISIM_NAMED_OBJECT

#include <QObject>
#include "exception.h"
#include "identifier.h"
#include "utilities.h"

namespace UniSim{

class NamedObject : public QObject
{
    Q_OBJECT
public:
    NamedObject(Identifier name, QObject *parent=0);
    Identifier id() const;
    QString fullName() const;

    template <class T> T peekOneChild(QString name);
    template <class T> T seekOneChild(QString name);
    template <class T> QList<T> seekChildren(QString name);

    template <class T> T peekParent(QString name);
    template <class T> T seekParent(QString name);

    template <class T> T peekOneNearest(QString name);
    template <class T> T seekOneNearest(QString name);

    template <class T> T peekOneSibling(QString name);
    template <class T> T seekOneSibling(QString name);
    template <class T> QList<T> seekSiblings(QString name);

    template <class T> T peekOneDescendant(QString name);
    template <class T> T seekOneDescendant(QString name);
    template <class T> QList<T> seekDescendants(QString name);

    template <class T> T peekOneAscendant(QString name);
    template <class T> T seekOneAscendant(QString name);

private:
	Identifier _id;
};

//! Finds exactly one child or none (n==1 || n==0)
template <class T> T NamedObject::peekOneChild(QString name) {
    return UniSim::peekOneChild<T>(name, this);
}

//! Finds exactly one child (n==1)
template <class T> T NamedObject::seekOneChild(QString name) {
    return UniSim::seekOneChild<T>(name, this);
}

//! Finds parent or not
template <class T> T NamedObject::peekParent(QString name) {
    return UniSim::peekParent<T>(name, this);
}

//! Finds parent
template <class T> T NamedObject::seekParent(QString name) {
    return UniSim::seekParent<T>(name, this);
}

//! Finds a number (n>=0) of children
template <class T> QList<T> NamedObject::seekChildren(QString name) {
    return UniSim::seekChildren<T>(name, this);
}

//! Finds exactly one nearest object or none (n==1 || n==0)
template <class T> T NamedObject::peekOneNearest(QString name) {
    return UniSim::peekOneNearest<T>(name, this);
}

//! Finds exactly one nearest object (n==1)
template <class T> T NamedObject::seekOneNearest(QString name) {
    return UniSim::seekOneNearest<T>(name, this);
}

//! Finds exactly one sibling or none (n==1 || n==0)
template <class T> T NamedObject::peekOneSibling(QString name) {
    if (!parent())
        return 0;
    T sibling = UniSim::peekOneChild<T>(name, parent());
    if (sibling == 0 || sibling == this)
        return 0;
    return sibling;
}

//! Finds exactly one sibling (n==1)
template <class T> T NamedObject::seekOneSibling(QString name) {
    T sibling = peekOneSibling<T>(name);
    if (!sibling)
        throw UniSim::Exception(objectName() + " has no sibling with name " + name);
    return sibling;
}

//! Finds a number (n>=0) of siblings
template <class T> QList<T> NamedObject::seekSiblings(QString name) {
    if (!parent())
        throw UniSim::Exception(objectName() + " has no siblings");
    QList<T> siblingsAndMe = UniSim::seekChildren<T>(name, parent());

    QList<T> siblings;
    for (int i=0; i < siblingsAndMe.size(); ++i) {
        T sib = siblingsAndMe[i];
        if (sib != this)
            siblings.append(sib);
    }
    return siblings;
}

//! Finds exactly one descendant or none (n==1 || n==0)
template <class T> T NamedObject::peekOneDescendant(QString name) {
    return UniSim::peekOneDescendant<T>(name, this);
}

//! Finds exactly one descendant (n==1)
template <class T> T NamedObject::seekOneDescendant(QString name) {
    return UniSim::seekOneDescendant<T>(name, this);
}

//! Finds a number (n>=0) of descendant
template <class T> QList<T> NamedObject::seekDescendants(QString name) {
    return UniSim::seekDescendants<T>(name, this);
}

//! Finds exactly one ascendant or none (n==1 || n==0)
template <class T> T NamedObject::peekOneAscendant(QString name) {
    return UniSim::peekOneAscendant<T>(name, this);
}

//! Finds exactly one ascendant (n==1)
template <class T> T NamedObject::seekOneAscendant(QString name) {
    return UniSim::seekOneAscendant<T>(name, this);
}


} //namespace


#endif
