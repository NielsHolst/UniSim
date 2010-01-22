/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UniSim_COMPONENT_H
#define UniSim_COMPONENT_H


#include <QMap>
#include <QObject>
#include "exception.h"
#include "identifier.h"
#include "utilities.h"

namespace UniSim{
	

class Component : public QObject
{
	Q_OBJECT
public:

    //! This enum describes the calling order of transformation functions.
    enum RecursionPolicy {
		ChildrenFirst, 	//!< First call children then parent (default).
		ChildrenLast, 	//!< First call parent then children.
		ChildrenNot		//!< Do not call children.
	};
	
    //! This enum identifies the transformation functions.
    enum Function {Initialize, Reset, Update, Cleanup, Debrief, AllFunctions};
	
    Component(Identifier name, QObject *parent=0);
		
    virtual void initialize() { }
    virtual void reset() { }
    virtual void update() { }
    virtual void cleanup() { }
    virtual void debrief() { }

    void deepInitialize();
    void deepReset();
    void deepUpdate();
    void deepCleanup();
    void deepDebrief();
	
    void setRecursionPolicy(Function function, RecursionPolicy policy);
    RecursionPolicy recursionPolicy(Function function) const;

    template <class T> QList<T> findChildren(QString name);
    template <class T> T findChild(QString name);
    template <class T> QList<T> findSiblings(QString name);
    template <class T> T findSibling(QString name);
    template <class T> T findAscendant(QString name);

signals:
    //! Signal provided for derived classes
    /*! Use this signal to broadcast events, such as
    \verbatim
    emit event(crop, "harvest");
    \endverbatim
    */
    void event(QObject *sender, QString eventName);

private:
    static QObject* _root;
    QMap<Function, RecursionPolicy> policy;
};

typedef QList<Component*> Components;

//! Finds any number of children
/*!
    Works like UniSim::find but searches only the children of this component.
    Note: This overrides QObject::findChildren<T> which is less flexible.
*/
template <class T> QList<T> Component::findChildren(QString name) {
    return UniSim::findChildren<T>(name, this);
}

//! Finds exactly one child
/*!
  Works like findChildren but throws an Exception if not exactly one child is found.
  Note: This overrides QObject::findChild<T> which is less flexible.
*/
template <class T> T Component::findChild(QString name) {
    return UniSim::findChild<T>(name, this);
}

//! Finds siblings
/*! Applies findChildren to parent
*/
template <class T> QList<T> Component::findSiblings(QString name) {
    if (!parent())
        throw UniSim::Exception(objectName() + " has no siblings");
    return UniSim::findChildren<T>(name, parent());
}

//! Finds exactly one sibling
/*! Applies findChild to parent
*/
template <class T> T Component::findSibling(QString name) {
    if (!parent())
        throw UniSim::Exception(objectName() + " has no siblings");
    return UniSim::findChild<T>(name, parent());
}

//! Finds an ascendant of this component or throws an exception
/*!
    Finds the most proximate ascendant matching name and type
*/
template <class T> T Component::findAscendant(QString name) {
    return UniSim::findAscendant<T>(name, this);
}


} //namespace


#endif
