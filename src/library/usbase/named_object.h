/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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

    template <class T> T seekOneChild(QString name);
    template <class T> QList<T> seekChildren(QString name);

    template <class T> T seekOneSibling(QString name);
    template <class T> QList<T> seekSiblings(QString name);

    template <class T> T seekOneDescendant(QString name);
    template <class T> QList<T> seekDescendants(QString name);

    template <class T> T seekOneAscendant(QString name);

private:
	Identifier _id;
};

//! Finds exactly one child (n==1)
template <class T> T NamedObject::seekOneChild(QString name) {
    return UniSim::seekOneChild<T>(name, this);
}

//! Finds a number (n>=0) of children
template <class T> QList<T> NamedObject::seekChildren(QString name) {
    return UniSim::seekChildren<T>(name, this);
}

//! Finds exactly one sibling (n==1)
template <class T> T NamedObject::seekOneSibling(QString name) {
    if (!parent())
        throw UniSim::Exception(objectName() + " has no siblings");
    T sibling = UniSim::seekOneChild<T>(name, parent());
    if (sibling == this)
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

//! Finds exactly one descendant (n==1)
template <class T> T NamedObject::seekOneDescendant(QString name) {
    return UniSim::seekOneDescendant<T>(name, this);
}

//! Finds a number (n>=0) of descendant
template <class T> QList<T> NamedObject::seekDescendants(QString name) {
    return UniSim::seekDescendants<T>(name, this);
}

//! Finds exactly one ascendant (n==1)
template <class T> T NamedObject::seekOneAscendant(QString name) {
    return UniSim::seekOneAscendant<T>(name, this);
}


} //namespace


#endif
