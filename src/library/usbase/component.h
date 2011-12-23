/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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

namespace UniSim{

class ParameterBase;
class PullVariableBase;


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
    void pullVariableChanged(PullVariableBase *var, ParameterBase *param);

private slots:
    void acceptPullVariableChanged(PullVariableBase *var, ParameterBase *param);

private:
    // data
    static QObject* _root;
    QMap<Function, RecursionPolicy> policy;
    QList<ParameterBase*> parameters;
};

typedef QList<Component*> Components;

} //namespace


#endif
