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
	
    QString fullName();

    void setRecursionPolicy(Function function, RecursionPolicy policy);
    RecursionPolicy recursionPolicy(Function function) const;

    template <class T> T seekOneChild(QString name);
    template <class T> QList<T> seekChildren(QString name);

    template <class T> T seekOneSibling(QString name);
    template <class T> QList<T> seekSiblings(QString name);

    template <class T> T seekOneDescendant(QString name);
    template <class T> QList<T> seekDescendants(QString name);

    template <class T> T seekOneAscendant(QString name);

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


//! Finds exactly one child (n==1)
template <class T> T Component::seekOneChild(QString name) {
    return UniSim::seekOneChild<T>(name, this);
}

//! Finds a number (n>=0) of children
template <class T> QList<T> Component::seekChildren(QString name) {
    return UniSim::seekChildren<T>(name, this);
}

//! Finds exactly one sibling (n==1)
template <class T> T Component::seekOneSibling(QString name) {
    if (!parent())
        throw UniSim::Exception(objectName() + " has no siblings");
    T sibling = UniSim::seekOneChild<T>(name, parent());
    if (sibling == this)
        throw UniSim::Exception(objectName() + " has no sibling with name " + name);
    return sibling;
}

//! Finds a number (n>=0) of siblings
template <class T> QList<T> Component::seekSiblings(QString name) {
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
template <class T> T Component::seekOneDescendant(QString name) {
    return UniSim::seekOneDescendant<T>(name, this);
}

//! Finds a number (n>=0) of descendant
template <class T> QList<T> Component::seekDescendants(QString name) {
    return UniSim::seekDescendants<T>(name, this);
}

//! Finds exactly one ascendant (n==1)
template <class T> T Component::seekOneAscendant(QString name) {
    return UniSim::seekOneAscendant<T>(name, this);
}


} //namespace


#endif
