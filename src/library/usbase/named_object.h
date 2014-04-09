/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
    //Q_OBJECT
public:
    NamedObject();
    NamedObject(Identifier name, QObject *parent=0);
    Identifier id() const;
    QString uniqueId() const;
    void setId(Identifier id);
    QString fullName() const;
    QString fullLabel() const;
    static void resetNumbering();
    long int number() const;

    NamedObject* root();
    QString absolutePath(QString relativePath);
    template <class T> QList<T> seekMany(QString name);
    template <class T> T peekOne(QString name);
    template <class T> T seekOne(QString name);

    template <class TParent, class TChild> QList<TChild> seekMany(QString expression);
    template <class TParent, class TChild> TChild seekOne(QString expression);

    template <class T> T peekOneChild(QString name);
    template <class T> T seekOneChild(QString name);
    template <class T> QList<T> seekChildren(QString name);

    template <class T> T peekParent(QString name);
    template <class T> T seekParent(QString name);

    template <class T> T peekOneNearest(QString name);
    template <class T> T seekOneNearest(QString name);
    template <class T> QList<T> seekManyNearest(QString name);
    template <class TParent, class TChild> QList<TChild> seekManyNearest(QString expression);

    template <class T> T peekOneSibling(QString name);
    template <class T> T seekOneSibling(QString name);
    template <class T> T peekPrecedingSibling(QString name);
    template <class T> T seekPrecedingSibling(QString name);
    template <class T> T peekFollowingSibling(QString name);
    template <class T> T seekFollowingSibling(QString name);
    template <class T> int seekSiblingPosition();
    template <class T> QList<T> seekSiblings(QString name, int *ixPreceding = 0);

    template <class T> T peekOneDescendant(QString name);
    template <class T> T seekOneDescendant(QString name);
    template <class T> QList<T> seekDescendants(QString name);

    template <class T> T peekOneAscendant(QString name);
    template <class T> T seekOneAscendant(QString name);
    template <class T> T peekNearestAscendant(QString name);
    template <class T> T seekNearestAscendant(QString name);
    template <class T> QList<T> seekAscendants(QString name);

private:
	Identifier _id;
    static long int objectCount;
    long int myNumber;
    template <class T> static QList<T> filterByName(QString name, const QList<QObject*> &candidates);
};

//! Finds a number (n>=0) of objects anywhere in the object tree
template <class T> QList<T> NamedObject::seekMany(QString name) {
    return root()->seekDescendants<T>(name);
}

//! Finds exactly one object, or none, anywhere in the object tree
template <class T> T NamedObject::peekOne(QString name) {
    return root()->peekOneDescendant<T>(name);
}

//! Finds exactly one object anywhere in the object tree
template <class T> T NamedObject::seekOne(QString name) {
 return root()->seekOneDescendant<T>(name);
}

//! Finds one parent-child object
/*!
    Uses seekMany<T,U> but throws an Exception if not exactly one match is found.
*/
template <class TParent, class TChild> TChild NamedObject::seekOne(QString expression)
{
    QList<TChild> result = seekMany<TParent, TChild>(expression);
    if (result.size() == 1 || expression.left(3) == "...")
        return result[0];

    QString head = result.isEmpty() ? "No object found matching:"
                                    : "More than one object found matching:";
    QString msg = "%1 '%2' with parent class '%3' and child class '%4'";
    throw Exception(msg
                    .arg(head)
                    .arg(expression)
                    .arg(typeid(TParent).name())
                    .arg(typeid(TChild).name()));
}

//! Finds any number of parent-child objects
/*!
  The path identifying the object(s) is identifying a parent-child relationship, the name of
  the child is given in brackets, e.g. "mammals/elephant[size]. The matching children are returned
  as a list.
*/
template <class TParent, class TChild> QList<TChild> NamedObject::seekMany(QString expression)
{
    if (expression.left(3) == "...") {
        if (expression[3] != '/') {
            QString msg{"Illegal path expression ('%1'). '/' expected after '...'"};
            throw UniSim::Exception(msg.arg(expression));
        }
        return seekManyNearest<TParent, TChild>(expression.mid(4));
    }
    QStringList split = UniSim::splitParentChildExpression( absolutePath(expression), this );
    QString parentName = split[0], childName = split[1];

    auto parents = seekMany<TParent>(parentName);
    QList<TChild> result;
    for (auto parent : parents)
        result << parent->seekChildren<TChild>(childName);
//    for (int i = 0; i < parentPtr.size(); ++i) {
//        QList<TChild> more = parentPtr[i]->seekChildren<TChild>(child);
//        result.append(more);
//    }
    return result;
}

//! Finds exactly one child or none (n==1 || n==0)
/*!
  Works like seekChildren but throws an Exception if not exactly one object, or none, is found
*/
template <class T> T NamedObject::peekOneChild(QString name) {
    QList<T> matches = seekChildren<T>(name);
    if (matches.size() == 0)
        return 0;
    else if (matches.size() > 1)
        throw Exception("'" + fullLabel() +
                        "' has more than one child called '" + name + "'" +
                        " of class " + typeid(T).name(), this);
    return matches[0];
}

//! Finds exactly one child (n==1)
/*!
  Works like seekChildren but throws an Exception if not exactly one object is found
*/
template <class T> T NamedObject::seekOneChild(QString name) {
    T result = peekOneChild<T>(name);
    if (!result)
        throw Exception("'" + fullLabel() +
                        "' has no child called '" + name + "'" +
                        " of class " + typeid(T).name(), this);
    return result;
}

//! Finds parent or not
template <class T> T NamedObject::peekParent(QString name) {
    T p = dynamic_cast<T>(parent());
    if (p) {
        QList<QObject*> candidates;
        candidates.append(p);
        QList<T> matches = filterByName<T>(name, candidates);
        if (matches.size() != 1)
            p = 0;
    }
    return p;
}

//! Finds parent
template <class T> T NamedObject::seekParent(QString name) {
    T p = peekParent<T>(name);
    if (!p) {
        QString msg = "Parent object '%1' of type '%2' not found";
        throw Exception(msg.arg(fullLabel()).arg(typeid(T).name()), this);
    }
    return p;
}

//! Finds a number (n>=0) of children
/*! Works like find but searches only the children of parent. Grandchildren and further descendant are not sought.
    (unlike Qt's findChildren which searches recursively and not only children).
*/
template <class T> QList<T> NamedObject::seekChildren(QString name) {
    QList<QObject*> candidates = children();
    return filterByName<T>(name, candidates);
}

//! Finds exactly one nearest object or none (n==1 || n==0)
template <class T> T NamedObject::peekOneNearest(QString name) {
    QList<NamedObject*> parents;
    parents << this << seekAscendants<NamedObject*>("*");
    for (auto parent : parents) {
        T child = parent->peekOneChild<T>(name);
        if (child) return child;
    }
//    for (int i = 0; i < parents.size(); ++i) {
//        T child = parents[i]->peekOneChild<T>(name);
//        if (child) return child;
//    }
    return 0;
}

//! Finds exactly one nearest object (n==1)
template <class T> T NamedObject::seekOneNearest(QString name) {
    T child = peekOneNearest<T>(name);
    if (child) return child;
    throw Exception("Found no nearest object called '" + name + "'" +
                    " of class " + typeid(T).name(), this);
}

template <class T> QList<T> NamedObject::seekManyNearest(QString name) {
    if (name.contains(".")) {
        QString msg{"Expression seeking for nearest model cannot be a relative path ('%1')"};
        throw UniSim::Exception(msg.arg(name), this);
    }
    QList<NamedObject*> parents;
    parents << this << seekAscendants<NamedObject*>("*");
    QList<T> nearest;
    for (auto parent : parents)
        nearest << parent->seekChildren<T>(name);
    return nearest;
}

template <class TParent, class TChild>
QList<TChild> NamedObject::seekManyNearest(QString expression) {
    QStringList split = UniSim::splitParentChildExpression(expression, this );
    QString parentName{split[0]}, childName{split[1]};

    auto parents = seekManyNearest<TParent>(parentName);
    QList<TChild> nearest;
    for (auto parent : parents)
        nearest << parent->seekChildren<TChild>(childName);
    return nearest;
}

//! Finds exactly one sibling or none (n==1 || n==0)
template <class T> T NamedObject::peekOneSibling(QString name) {
    if (!parent())
        return 0;
    NamedObject *parent = seekParent<NamedObject*>("*");
    T sibling = parent->peekOneChild<T>(name);
    if (sibling == 0 || sibling == this)
        return 0;
    return sibling;
}

//! Finds exactly one sibling (n==1)
template <class T> T NamedObject::seekOneSibling(QString name) {
    T sibling = peekOneSibling<T>(name);
    if (!sibling)
        throw UniSim::Exception("No sibling with name " + name +
                                " of class " + typeid(T).name(), this);
    return sibling;
}

//! Finds preceding sibling or none (n==1 || n==0)
template <class T> T NamedObject::peekPrecedingSibling(QString name) {
    int ixPreceding;
    QList<T> siblings = seekSiblings<T>(name, &ixPreceding);
    return ixPreceding == -1 ? 0 : siblings.at(ixPreceding);
}

//! Finds preceding sibling (n==1)
template <class T> T NamedObject::seekPrecedingSibling(QString name) {
    T sibling = peekPrecedingSibling<T>(name);
    if (!sibling)
        throw UniSim::Exception("No preceding sibling with name " + name +
                                " of class " + typeid(T).name(), this);
    return sibling;
}

//! Finds following sibling or none (n==1 || n==0)
template <class T> T NamedObject::peekFollowingSibling(QString name) {
    int ixPreceding, ixFollowing;
    QList<T> siblings = seekSiblings<T>(name, &ixPreceding);
    ixFollowing = ixPreceding + 1;
    return ixFollowing >= siblings.size() ? 0 : siblings.at(ixFollowing);
}

//! Finds following sibling (n==1)
template <class T> T  NamedObject::seekFollowingSibling(QString name) {
    T sibling = peekFollowingSibling<T>(name);
    if (!sibling)
        throw UniSim::Exception("No following sibling with name " + name +
                                " of class " + typeid(T).name(), this);
    return sibling;
}
//! Finds my position among siblings
template <class T> int NamedObject::seekSiblingPosition() {
    int ixPreceding;
    seekSiblings<T>("*", &ixPreceding);
    return ixPreceding + 1;
}

//! Finds a number (n>=0) of siblings
template <class T> QList<T> NamedObject::seekSiblings(QString name, int *ixPreceding) {
    QList<T> siblingsAndMe;
    if (!parent())
        return siblingsAndMe;
    NamedObject *parent = seekParent<NamedObject*>("*");
    siblingsAndMe = parent->seekChildren<T>(name);

    QList<T> siblings;
    for (int i=0; i < siblingsAndMe.size(); ++i) {
        T sib = siblingsAndMe[i];
        if (sib == this) {
            if (ixPreceding) *ixPreceding = i-1;
        }
        else {
            siblings.append(sib);
        }
    }
    return siblings;
}

//! Finds exactly one descendant (n==1)
/*!
  Works like seekDescendants but throws an Exception if not exactly one object is found
*/
template <class T> T NamedObject::seekOneDescendant(QString name) {
    T result = peekOneDescendant<T>(name);
    if (!result)
        throw Exception("No descendant called '" + name + "'" +
                        " of class " + typeid(T).name(), this);
    return result;
}

//! Finds a number (n>=0) of descendant
/*!
  The path identifying the object(s) can be atomic ("elephant"),
  or, it can specify a path relative to the root ("mammals/elephant").
  Jokers are also allowed ("*", "mammals/ *", "* /leaves)
*/
template <class T> QList<T> NamedObject::seekDescendants(QString name) {
    QList<QObject*> candidates = findChildren<QObject*>();
    return filterByName<T>(name, candidates);
}

//! Finds exactly one ascendant or none (n==1 || n==0)
/*!
  Works like seekDescendants but throws an Exception, if not exactly one object or none is found. In the latter
  case a null pointer is returned
*/
 template <class T> T NamedObject::peekOneDescendant(QString name) {
    QList<T> matches = seekDescendants<T>(name);
    if (matches.size() == 0)
        return 0;
    else if (matches.size() > 1)
        throw Exception("More than one descendant called '" + name + "'" +
                        " of class " + typeid(T).name(), this);
    return matches[0];
}

//! Finds an ascendant or none (n==1 || n==0)
/*!
    Finds one ascendant matching name and type, or throws an Exception
*/
template <class T> T NamedObject::peekOneAscendant(QString name) {
    QList<T> ascendants = seekAscendants<T>(name);;
    if (ascendants.size() == 0)
        return 0;
    if (ascendants.size() > 1)
        throw Exception("More than one ascendant called '" + name +"'" +
                        " of class " + typeid(T).name(), this);
    return ascendants[0];
}

//! Finds exactly one ascendant (n==1)
/*!
    Finds one ascendant matching name and type, or throws an Exception
*/
template <class T> T NamedObject::seekOneAscendant(QString name) {
    T result = peekOneAscendant<T>(name);
    if (!result)
        throw Exception("No ascendants called '" + name + " of class " + typeid(T).name(), this);
    return result;
}

//! Finds nearest ascendant or none (n==1 || n==0)
/*!
    Finds nearest ascendant matching name and type, or null
*/
template <class T> T NamedObject::peekNearestAscendant(QString name) {
    QList<T> ascendants = seekAscendants<T>(name);;
    return ascendants.isEmpty() ? 0 : ascendants[0];
}

//! Finds nearest ascendant (n==1)
/*!
    Finds nearest ascendant matching name and type, or throws an Exception
*/
template <class T> T NamedObject::seekNearestAscendant(QString name) {
    T result = peekNearestAscendant<T>(name);
    if (!result)
        throw Exception("No ascendants called '" + name + " of class " + typeid(T).name(), this);
    return result;
}

//! Finds a number (n>=0) of ascendants
/*!
    Finds all ascendants matching name and type
*/
template <class T> QList<T> NamedObject::seekAscendants(QString name) {
    QList<QObject*> candidates;
    QObject *p = parent();
    while (p) {
        candidates.append(p);
        p = p->parent();
    }

    return filterByName<T>(name, candidates);

}

//! Selects objects in candidate list matching name
template <class T> QList<T> NamedObject::filterByName(QString name, const QList<QObject*> &candidates) {
    QList<T> result;
    QStringList names = Identifier(name).key().split("/");
    if (!names.isEmpty() && names.first().isEmpty())
        names.removeFirst();
    if (!names.isEmpty() && names.last().isEmpty())
        names.removeLast();
    if (names.isEmpty())
        return result;

    for (auto candidate : candidates) {
//    for (int ca = 0; ca < candidates.size(); ++ca) {
        int i = names.size()-1;
        QObject *p{candidate};
//        QObject *candidate, *p;
//        candidate = p = candidates.at(ca);
        while (i > 0 && p->parent() && (p->objectName() == names[i] || names[i] == "*")) {
            --i;
            p = p->parent();
        }
        if (names[i].isEmpty())
            throw Exception("Composite component name contains an empty name: " + name, candidate);
        bool isT = dynamic_cast<T>(candidate) != 0;
        bool pathOk  =	i == 0 && (p->objectName() == names[0] || names[0] == "*");
        if (isT && pathOk) {
            result << dynamic_cast<T>(candidate);
        }
    }
    return result;
}


} //namespace


#endif
