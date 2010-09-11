/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_UTILITIES_H
#define UniSim_UTILITIES_H

#include <cmath>
#include <cfloat>
#include <QDate>
#include <QDir>
#include <QList>
#include <QMap>
#include <QObject>
#include <QPluginLoader>
#include <QString>
#include <QStringList>
#include <QVariant>
#include "exception.h"
#include "file_locations.h"
#include "identifier.h"
#include "object_pool.h"

#include <iostream>
using std::cout;

namespace UniSim {

const double PI = 3.14159265;
const double DEG_TO_RAD = 0.017453293;
const double MAX_EXP = -log(DBL_EPSILON);
const double MIN_EXP = -log(1. - DBL_EPSILON);

class Model;

//! @name Versioning
//@{
QString version();
bool isDeveloperVersion();

//@}

//! @name Navigation
//@{
void setSimulationObjectFromDescendent(QObject *object);

template <class T> T seekOne(QString name);
template <class T> QList<T> seekMany(QString name);

template <class TParentPtr, class TChildPtr> TChildPtr seekOne(QString expression);
template <class TParentPtr, class TChildPtr> QList<TChildPtr> seekMany(QString expression);

template <class T> T seekOneChild(QString name, QObject *parent);
template <class T> QList<T> seekChildren(QString name, QObject *parent);

template <class T> T seekOneDescendant(QString name, QObject *root);
template <class T> QList<T> seekDescendants(QString name, QObject *root);

template <class T> T seekOneAscendant(QString name, QObject *child);
template <class T> T seekFirstAscendant(QString name, QObject *child);
template <class T> QList<T> seekAscendants(QString name, QObject *child);

template <class T> QList<T> filterByName(QString name, const QList<QObject*> &candidates);
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
double interpolate(const QMap<int, double> xy, int x);
double pow0(double x, double c);
double negExp(double x);
double GBFuncResp(double demand, double apparency);
int toDayOfYear(int day, int month);
//@}

//! @name String handling
//@{
QString fullName(const QObject *object);
QStringList decodeSimpleList(QString parenthesizedList, QString errorContext = QString());
void splitAtNamespace(QString s, QString *namespacePart, QString *ownNamePart);
QStringList splitParentChildExpression(QString expression);
template<class T> T stringToValue(QString s);
template<> bool stringToValue<bool>(QString s);
template<> QDate stringToValue<QDate>(QString s);
//@}

//! @name PlugIn handling
//@{
template <class TPlugin>
void lookupPlugIns(QString makerId, QMap<Identifier, TPlugin*> *makers);
//@}

//! @name Testing
//@{
void writeObjectTree(QObject *root, int level = 0);
void writeStandardTestFile(QString filePath);
//@}

// ========================
// Template implementations

template<class T> T stringToValue(QString s) {
    QVariant var(s.trimmed());
    if (!var.canConvert<T>()) {
        QString msg = "Cannot convert '" + s + "' to type " + QVariant(T()).typeName();
        throw Exception(msg);
    }
    return var.value<T>();
}


//! Finds any number of objects
/*!
  The path identifying the object(s) can be atomic ("elephant"),
  or, it can specify a path relative to the root ("mammals/elephant").
  Jokers are also allowed ("*", "mammals/ *", "* /leaves)
*/
template <class T> QList<T> seekDescendants(QString name, QObject *root) {
    QObject *useRoot = root ? root : simulationObject();
    QList<QObject*> candidates = useRoot->findChildren<QObject*>();
    if (!root)
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
    if (result.isEmpty())
        throw Exception("No object found matching: " + expression);
    else if (result.size() > 1)
        throw Exception("More than one object found matching: " + expression);
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

//! Finds exactly one object
/*!
  Works like find but throws an Exception if not exactly one object is found
*/
 template <class T> T seekOneDescendant(QString name, QObject *root) {
    QObject *useRoot = root ? root : simulationObject();
    QList<T> matches = seekDescendants<T>(name, useRoot);
    if (matches.size() == 0)
        throw Exception("'" + useRoot->objectName() +
                        "' has no descendant called '" + name + "'", useRoot);
    else if (matches.size() > 1)
        throw Exception("'" + useRoot->objectName() +
                        "' has more than one descendant called '" + name + "'", useRoot);
    return matches[0];
}

 //! Finds exactly one object (n==0) anywhere in simulation object tree
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
    QList<QObject*> candidates = useParent->children();
    return filterByName<T>(name, candidates);
}

//! Finds exactly one child
/*!
  Works like findChildren but throws an Exception if not exactly one object is found
*/
template <class T> T seekOneChild(QString name, QObject *parent) {
    QObject *useParent = parent ? parent : simulationObject();
    QList<T> matches = seekChildren<T>(name, useParent);
    if (matches.size() == 0)
        throw Exception("'" + useParent->objectName() +
                        "' has no child called '" + name + "'", useParent );
    else if (matches.size() > 1)
        throw Exception("'" + useParent->objectName() +
                        "' has more than one child called '" + name + "'", useParent);
    return matches[0];
}

//! Finds an ascendant of child or throws an exception
/*!
    Finds one ascendant matching name and type, or throws an Exception
*/
template <class T> T seekOneAscendant(QString name, QObject *child) {
    QList<T> ascendants = seekAscendants<T>(name, child);;
    if (ascendants.size() == 0)
        throw Exception("No ascendants called '" +
                        name + "', or it is not of the expected type", child);
    if (ascendants.size() > 1)
        throw Exception("More than one ascendant called '" + name +"'", child);
    return ascendants.at(0);
}

//! Finds first ascendant of child or throws an exception
/*!
    Finds first (nearest) ascendant matching name and type, or throws an Exception
*/
template <class T> T seekFirstAscendant(QString name, QObject *child) {
    QList<T> ascendants = seekAscendants<T>(name, child);;
    if (ascendants.size() == 0)
        throw Exception("No ascendants called '" +
                        name + "', or it is not of the expected type", child);
    return ascendants.at(0);
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

template <class TPlugin>
void lookupPlugIns(QString makerId, QMap<Identifier, TPlugin*> *makers) {
    bool keepLooking = true;
    do {
        QDir dir = FileLocations::location(FileLocationInfo::Plugins);
        foreach (QString filename, dir.entryList(QDir::Files)) {

            QPluginLoader loader(dir.absoluteFilePath(filename));

            TPlugin *plugin = qobject_cast<TPlugin*>(loader.instance());
            if (plugin) {
                plugin->useObjectPool(objectPool());
				QList<Identifier> classes = plugin->supportedClasses().keys();
                foreach (Identifier id, classes) {
                    (*makers)[id] = plugin;
                    Identifier idWithNamespace = plugin->pluginName().label() + "::" + id.label();
                    (*makers)[idWithNamespace] = plugin;

                }
            }
        }

        if (makers->size() > 0) {
            keepLooking = false;
        }
        else {
            QString msg = "Found no plugins for: " + makerId + " in: " + dir.absolutePath();
            if (!dir.exists())
                msg += ".\nThe folder does not exist.";
            keepLooking = FileLocations::lookup(FileLocationInfo::Plugins, msg);
        }
    } while (keepLooking);
}

} //namespace

#endif
