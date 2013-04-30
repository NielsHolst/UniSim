/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_COMPONENT_H
#define UNISIM_COMPONENT_H

#include <QList>
#include <QMap>
#include <QObject>
#include "named_object.h"
#include "parameter.h"
#include "ref.h"
#include "variable.h"

namespace UniSim{

class ParameterBase;

class Component : public NamedObject
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
    enum Function {Amend, Initialize, Reset, Update, Cleanup, Debrief, AllFunctions};
	
    Component(Identifier name, QObject *parent=0);
		
    template <class T>
    void addParameter(Identifier name, T *valuePtr, T defaultvalue, QString desc);

    template <class T>
    void addParameterRef(Identifier name, T *valuePtr, QString reference);

    template <class T>
    void addVariable(Identifier name, T *valuePtr, QString desc);

    virtual void amend() { }
    virtual void initialize() { }
    virtual void reset() { }
    virtual void update() { }
    virtual void cleanup() { }
    virtual void debrief() { }

    void deepAmend();
    void deepInitialize();
    void deepReset();
    void deepUpdate();
    void deepCleanup();
    void deepDebrief();

    void followRedirections();

    void setRecursionPolicy(Function function, RecursionPolicy policy);
    RecursionPolicy recursionPolicy(Function function) const;

signals:
    //! Signal provided for derived classes
    /*! Use this signal to broadcast events, such as
    \verbatim
    emit event(crop, "harvest");
    \endverbatim
    */
    void event(QObject *sender, QString eventName);
//    void pullValueChanged(PullVariableBase *var, ParameterBase *param);

//private slots:
//    void acceptPullVariableChanged(PullVariableBase *var, ParameterBase *param);

private:
    // data
    static QObject* _root;
    QMap<Function, RecursionPolicy> policy;
    QList<ParameterBase*> parameters;
};

typedef QList<Component*> Components;

template <class T>
void Component::addParameter(Identifier name, T *valuePtr, T defaultvalue, QString desc) {
    new Parameter<T>(name, valuePtr, defaultvalue, this, desc);
}

template <class T>
void Component::addParameterRef(Identifier name, T *valuePtr, QString reference) {
    Parameter<T> *par = new Parameter<T>(name, valuePtr, T(), this, reference);
    new Ref(this, par, reference);
}

template <class T>
void Component::addVariable(Identifier name, T *valuePtr, QString desc) {
    new Variable<T>(name, valuePtr, this, desc);
}

} //namespace


#endif
