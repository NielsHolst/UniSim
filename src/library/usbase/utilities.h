/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_UTILITIES_H
#define UNISIM_UTILITIES_H

#include <cmath>
#include <cfloat>
#include <iostream>
#include <limits>
#include <typeinfo>
#include <QDate>
#include <QDir>
#include <QList>
#include <QMap>
#include <QObject>
#include <QPluginLoader>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QVariant>
#include "exception.h"
#include "file_locations.h"
#include "identifier.h"

using std::cout;

namespace UniSim {

const double PI = 3.14159265;
const double DEG_TO_RAD = 0.017453293;
const double MAX_EXP = -log(DBL_EPSILON);
const double MIN_EXP = -log(1. - DBL_EPSILON);

class Model;

//! @name Navigation
//@{
void setSimulationObjectFromDescendent(QObject *object);

template <class T> T peekOne(QString name);
template <class T> T seekOne(QString name);
template <class T> QList<T> seekMany(QString name);

template <class TParentPtr, class TChildPtr> TChildPtr seekOne(QString expression);
template <class TParentPtr, class TChildPtr> QList<TChildPtr> seekMany(QString expression);

template <class T> T peekOneNearest(QString name, QObject *parent);
template <class T> T seekOneNearest(QString name, QObject *parent);

template <class T> T peekOneChild(QString name, QObject *parent);
template <class T> T seekOneChild(QString name, QObject *parent);
template <class T> QList<T> seekChildren(QString name, QObject *parent);

template <class T> T peekParent(QString name, QObject *object);
template <class T> T seekParent(QString name, QObject *object);

template <class T> T peekOneDescendant(QString name, QObject *root);
template <class T> T seekOneDescendant(QString name, QObject *root);
template <class T> QList<T> seekDescendants(QString name, QObject *root);

template <class T> T peekOneAscendant(QString name, QObject *child);
template <class T> T seekOneAscendant(QString name, QObject *child);
template <class T> T peekNearestAscendant(QString name, QObject *child);
template <class T> T seekNearestAscendant(QString name, QObject *child);
template <class T> QList<T> seekAscendants(QString name, QObject *child);

template <class T> QList<T> filterByName(QString name, const QList<QObject*> &candidates);
//@}

//! @name File handling
//@{
QFileInfo findNearestFile(QDir home, QString subFolder, QString fileName);
//@}


//! \cond
class SimulationObject {
    private:
    static QObject* simulation;
    friend void setSimulationObject(QObject *simulation);
    friend void setSimulationObjectFromDescendent(QObject *descendent);
    friend QObject* simulationObject();
};
void setSimulationObject(QObject *simulation);
void setSimulationObjectFromDescendent(QObject *descendent);
QObject* simulationObject();
//! \endcond

//! @name Mathematics
//@{
double accum(const QVector<double> &x);
double interpolate(const QMap<int, double> xy, int x);
double pow0(double x, double c);
double negExp(double x);
double divBounded(double x, double y, double bound = std::numeric_limits<double>::max());
double GBFuncResp(double demand, double supply);
int toDayOfYear(int day, int month);
//@}

//! @name String handling
//@{
QString fullName(const QObject *object);
void chopParentheses(QString &s, QObject *concerning = 0);
void chopRightParenthesis(QString &s, QObject *concerning = 0);
QStringList decodeSimpleList(QString parenthesizedList, QObject *concerning = 0);  // deprecated
QStringList decodeList(QString s, QObject *context = 0);
template<class T> QList<T> decodeList(QString s, QObject *context = 0);
template<class T, class U> QMap<T,U> decodeList(QString s, QObject *context = 0);
void splitAtNamespace(QString s, QString *namespacePart, QString *ownNamePart);
QStringList splitParentChildExpression(QString expression);
template <class T> QList< QPair<QString, T> > decodeNameValueList(QString nameValueList, QObject *concerning = 0);

template<class T> T stringToValue(QString s, QObject *concerning = 0);
template<> QString stringToValue<QString>(QString s, QObject *concerning);
template<> double stringToValue<double>(QString s, QObject *concerning);
template<> int stringToValue<int>(QString s, QObject *concerning);
template<> char stringToValue<char>(QString s, QObject *concerning);
template<> bool stringToValue<bool>(QString s, QObject *concerning);
template<> QDate stringToValue<QDate>(QString s, QObject *concerning);
template<> QTime stringToValue<QTime>(QString s, QObject *concerning);

template<class T> QString valueToString(T value);
template<> QString valueToString<char>(char value);
template<> QString valueToString<bool>(bool value);
template<> QString valueToString<QDate>(QDate value);
template<> QString valueToString<QTime>(QTime value);

template <class T> bool isType(QString s);
//@}

//! @name Testing
//@{
void writeObjectTree(QObject *root, int level = 0);
void writeStandardTestFile(QString filePath);
//@}

// ========================
// Template implementations

template<class T> T stringToValue(QString s, QObject *concerning) {
//    static_assert( false, "that was false" );
    QString msg = "stringToValue does not support this type: '%1' of type '%2'";
    throw Exception(msg.arg(s).arg(QVariant(T()).typeName()), concerning);
//    QVariant var(s.trimmed());
//    if (!var.canConvert<T>()) {
//        QString msg = "Cannot convert '" + s + "' to type " + QVariant(T()).typeName();
//        throw Exception(msg, concerning);
//    }
//    return var.value<T>();
}

template<class T> QString valueToString(T value) {
    return QVariant(value).toString();
}

template <class T> bool isType(QString s) {
    bool ok = true;
    try {
        stringToValue<T>(s);
    }
    catch (Exception &) {
        ok = false;
    }
    return ok;
}



//! Finds any number of objects
/*!
  The path identifying the object(s) can be atomic ("elephant"),
  or, it can specify a path relative to the root ("mammals/elephant").
  Jokers are also allowed ("*", "mammals/ *", "* /leaves)
*/
template <class T> QList<T> seekDescendants(QString name, QObject *root) {
    QObject *useRoot = root ? root : simulationObject();
    if (!useRoot)
        return QList<T>();
    QList<QObject*> candidates = useRoot->findChildren<QObject*>();
    if (!root && useRoot) // if no root then make search truely global by including the useRoot too
        candidates.prepend(useRoot);
    return filterByName<T>(name, candidates);
}

//! Finds a number (n>=0) of objects anywhere in simulation object tree
template <class T> QList<T> seekMany(QString name) {
    return seekDescendants<T>(name, 0);
}

//! Finds one parent-child object
/*!
    Uses seekMany<T,U> but throws an Exception if not exactly one match is found.
*/
template <class TParentPtr, class TChildPtr> TChildPtr seekOne(QString expression) {
    QList<TChildPtr> result = seekMany<TParentPtr, TChildPtr>(expression);
    QString msg = expression +
                  " with parent class: " + typeid(TParentPtr).name() +
                  " and child class: " + typeid(TChildPtr).name();
    if (result.isEmpty())
        throw Exception("No object found matching: " + msg);
    else if (result.size() > 1)
        throw Exception("More than one object found matching: " + msg);
    return result[0];
}

//! Finds any number of parent-child objects
/*!
  The path identifying the object(s) is identifying a parent-child relationship, the name of
  the child is given in brackets, e.g. "mammals/elephant[size]. The matching children are returned
  as a list.
*/
template <class TParentPtr, class TChildPtr> QList<TChildPtr> seekMany(QString expression) {
    QStringList split = splitParentChildExpression(expression);
    QString parent = split[0], child = split[1];

    QList<TParentPtr> parentPtr = seekMany<TParentPtr>(parent);
    QList<TChildPtr> result;
    for (int i = 0; i < parentPtr.size(); ++i) {
        QList<TChildPtr> more = seekChildren<TChildPtr>(child, parentPtr[i]);
        result.append(more);
    }
    return result;
}

//! Finds exactly one object, or none
/*!
  Works like seekDescendants but throws an Exception if not exactly one object or none is found
*/
 template <class T> T peekOneDescendant(QString name, QObject *root) {
    QList<T> matches = seekDescendants<T>(name, root);
    QObject *useRoot = root ? root : simulationObject();
    if (matches.size() == 0)
        return 0;
    else if (matches.size() > 1)
        throw Exception("More than one descendant called '" + name + "'" +
                        " of class " + typeid(T).name(), useRoot);
    return matches[0];
}

 //! Finds exactly one object
/*!
  Works like seekDescendants but throws an Exception if not exactly one object is found
*/
template <class T> T seekOneDescendant(QString name, QObject *root) {
    QObject *useRoot = root ? root : simulationObject();
    T result = peekOneDescendant<T>(name, root);
    if (!result)
        throw Exception("No descendant called '" + name + "'" +
                        " of class " + typeid(T).name(), useRoot);
    return result;
}

//! Finds exactly one object, or none, anywhere in simulation object tree
template <class T> T peekOne(QString name) {
    return peekOneDescendant<T>(name, 0);
}

//! Finds exactly one object anywhere in simulation object tree
template <class T> T seekOne(QString name) {
 return seekOneDescendant<T>(name, 0);
}

 //! Finds any number of children
/*!
    Works like find but searches only the children of parent. Grandchildren and further descendant are not sought.
    (unlike Qt's findChildren which searches recursively and not only children).

    If parent is null then the root is the assumed parent,
*/

template <class T> QList<T> seekChildren(QString name, QObject *parent) {
    QObject *useParent = parent ? parent : simulationObject();
    if (!useParent)
        return QList<T>();
    QList<QObject*> candidates = useParent->children();
    return filterByName<T>(name, candidates);
}

template <class T> T peekParent(QString name, QObject *object) {
    T p = dynamic_cast<T>(object->parent());
    if (p) {
        QList<QObject*> candidates;
        candidates.append(p);
        QList<T> matches = filterByName<T>(name, candidates);
        if (matches.size() != 1)
            p = 0;
    }
    return p;
}

template <class T> T seekParent(QString name, QObject *object) {
    T p = peekParent<T>(name, object);
    if (!p)
        throw Exception("Cound not find parent: " +  name +
                        " of class " + typeid(T).name(), object);
    return p;
}


template <class T> T peekOneNearest(QString name, QObject *parent) {
    QList<QObject*> parents;
    parents.append(parent);
    parents.append(seekAscendants<QObject*>("*", parent));
    for (int i = 0; i < parents.size(); ++i) {
        T child = peekOneChild<T>(name, parents[i]);
        if (child) return child;
    }
    return 0;
}

template <class T> T seekOneNearest(QString name, QObject *parent) {
    T child = peekOneNearest<T>(name, parent);
    if (child) return child;
    throw Exception("Found no nearest object called '" + name + "'" +
                    " of class " + typeid(T).name(), parent);
}


//! Finds exactly one child, or none
/*!
  Works like seekChildren but throws an Exception if not exactly one object, or none, is found
*/
template <class T> T peekOneChild(QString name, QObject *parent) {
    QObject *useParent = parent ? parent : simulationObject();
    QList<T> matches = seekChildren<T>(name, useParent);
    if (matches.size() == 0)
        return 0;
    else if (matches.size() > 1)
        throw Exception("'" + useParent->objectName() +
                        "' has more than one child called '" + name + "'" +
                        " of class " + typeid(T).name(), useParent);
    return matches[0];
}

//! Finds exactly one child
/*!
  Works like findChildren but throws an Exception if not exactly one object is found
*/
template <class T> T seekOneChild(QString name, QObject *parent) {
    QObject *useParent = parent ? parent : simulationObject();
    T result = peekOneChild<T>(name, parent);
    if (!result)
        throw Exception("'" + useParent->objectName() +
                        "' has no child called '" + name + "'" +
                        " of class " + typeid(T).name(), useParent );
    return result;
}

//! Finds an ascendant of child or none
/*!
    Finds one ascendant matching name and type, or throws an Exception
*/
template <class T> T peekOneAscendant(QString name, QObject *child) {
    QList<T> ascendants = seekAscendants<T>(name, child);;
    if (ascendants.size() == 0)
        return 0;
    if (ascendants.size() > 1)
        throw Exception("More than one ascendant called '" + name +"'" +
                        " of class " + typeid(T).name(), child);
    return ascendants.at(0);
}

//! Finds an ascendant of child or throws an exception
/*!
    Finds one ascendant matching name and type, or throws an Exception
*/
template <class T> T seekOneAscendant(QString name, QObject *child) {
    T result = peekOneAscendant<T>(name, child);
    if (!result)
        throw Exception("No ascendants called '" + name + " of class " + typeid(T).name(), child);
    return result;
}

//! Finds nearest ascendant of child or none
/*!
    Finds nearest ascendant matching name and type, or null
*/
template <class T> T peekNearestAscendant(QString name, QObject *child) {
    QList<T> ascendants = seekAscendants<T>(name, child);;
    return ascendants.isEmpty() ? 0 : ascendants.at(0);
}

//! Finds nearest ascendant of child or throws an exception
/*!
    Finds nearest ascendant matching name and type, or throws an Exception
*/
template <class T> T seekNearestAscendant(QString name, QObject *child) {
    T result = peekNearestAscendant<T>(name, child);
    if (!result)
        throw Exception("No ascendants called '" + name + " of class " + typeid(T).name(), child);
    return result;
}


//! Finds all ascendants of child
/*!
    Finds all ascendants matching name and type
*/
template <class T> QList<T> seekAscendants(QString name, QObject *child) {
    if (!child)
        throw Exception("seekFirstAscendant called with null pointer");

    QList<QObject*> candidates;
    QObject *p = child->parent();
    while (p) {
        candidates.append(p);
        p = p->parent();
    }

    return filterByName<T>(name, candidates);
}

//! Selects objects in candidate list matching name
template <class T> QList<T> filterByName(QString name, const QList<QObject*> &candidates) {
    QList<T> result;
    QStringList names = Identifier(name).key().split("/");
    if (!names.isEmpty() && names.first().isEmpty())
        names.removeFirst();
    if (!names.isEmpty() && names.last().isEmpty())
        names.removeLast();
    if (names.isEmpty())
        return result;

    for (int ca = 0; ca < candidates.size(); ++ca) {
        int i = names.size()-1;
        QObject *candidate, *p;
        candidate = p = candidates.at(ca);
        while (i > 0 && p->parent() && (p->objectName() == names[i] || names[i] == "*")) {
            --i;
            p = p->parent();
        }
        if (names[i].isEmpty())
            throw Exception("Composite component name contains an empty name: " + name, candidate);
        bool isT = dynamic_cast<T>(candidate) != 0;
        bool pathOk  =	i == 0 && (p->objectName() == names[0] || names[0] == "*");
        if (isT && pathOk) {
            result.append(dynamic_cast<T>(candidate));
        }
    }
    return result;
}

//
// String handling
//

template<class T>
QList<T> decodeList(QString s, QObject *context = 0) {
    QList<T> result;
    QStringList strings = decodeList(s, context);
    for (int i = 0; i < strings.size(); ++i) {
        T value = stringToValue<T>(strings[i], context);
        result.append(value);
    }
    return result;
}

template<class T, class U>
QMap<T,U> decodeList(QString s, QObject *context = 0) {
    QMap<T,U> result;
    QStringList pairs = decodeList(s, context);
    for (int i = 0; i < pairs.size(); ++i) {
        QStringList pair = decodeList(pairs[i]);
        if (pair.size() != 2)
            throw Exception("(name value) pair expected, got: \"" + pairs[i] + "\"", context);
        T key = stringToValue<T>(pair[0], context);
        U value = stringToValue<U>(pair[1], context);
        if (result.contains(key))
            throw Exception("Name must be unique in list, found \"" + pair[0] + "\" twice");
        result[key] = value;
    }
    return result;
}

template <class T>
QList< QPair<QString, T> > decodeNameValueList(QString nameValueList, QObject *concerning) {
    QList< QPair<QString, T> > result;
    QString s = nameValueList.simplified();
    s = s.trimmed();
    if (s.size() == 0) throw Exception("Name-value list is empty", concerning);
    if (s.left(1) != "(") throw Exception("Name-value list must begin with '(': " + s, concerning);

    QStringList parts = s.split("(");
    for (int i = 0; i < 2; ++i) {
        if (parts[i].size() > 0 && parts[i].left(1) != " ")
            throw UniSim::Exception("Name-value list must begin with two left parentheses: " + s, concerning);
    }

    if (parts.size() < 3)
        throw UniSim::Exception("Name-value list is incomplete: " + s, concerning);

    for (int i = 2; i< parts.size(); ++i) {
        QString part = parts[i].trimmed();
        chopRightParenthesis(part, concerning);
        if (i == parts.size() - 1)
            chopRightParenthesis(part, concerning);

        QStringList strPair = part.split(" ");
        bool ok = strPair.size() == 2;
        if (ok) {
            QPair<QString, T> pair;
            pair.first = strPair[0];
            pair.second = stringToValue<T>(strPair[1], concerning);
            result.append(pair);
        }

        if (!ok)
            throw UniSim::Exception("Name-value list must contain (name value) pairs: " + s, concerning);
    }
    return result;
}

} //namespace

#endif
