/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QString>
#include "component.h"
#include "exception.h"
#include "parameter_base.h"
#include "pull_variable_base.h"
#include "utilities.h"

/*! \class UniSim::Component
    \brief The %Component class provides the basic building block to construct models.

    Component is used as a base class to derive specific building blocks for your own modelling. The derived
    classes will usually add data members to represent parameters, state and input variables.

    Components follow the composite pattern by which Component objects are arranged in parent-child
    relationships.

    The state of a Component should only be changed through its five transformation functions:
    initialize(), reset(), update(), cleanup() and debrief().

    The transformation functions also exist in a recursive version (with a 'deep' in the function
    name, in which the invocation is forwarded recursively to the children and their descendants.
    You can control the order of invocation for each parent-child relationship by setting the RecursionPolicy.

    Upon creation and subsequent initialization of all components, the transformations are effected by
    Simulation::execute:

    \verbatim
    client code {
        .
        .
        create all components;
        initialize all components;
        simulation->execute();
        .
        .
    }

    Simulation::execute() {
        loop {
            reset all components;
            loop {
                update all components;
            }
            cleanup all components;
        }
        debrief all components;
    }
    \endverbatim

    Thus you program the functionality of your derived components by redefining any of the five
    transformation functions which are all defined empty in the Component class.
*/

namespace UniSim{

Component::Component(Identifier name, QObject *parent)
    : NamedObject(name, parent)
{
    setRecursionPolicy(Component::AllFunctions, Component::ChildrenFirst);
}

namespace {
	
	typedef void (Component::*FunctionPtr)();
	
    void call(Component *p, FunctionPtr doIt, FunctionPtr doItDeep, Component::RecursionPolicy policy)
	{
		if (policy == Component::ChildrenNot) {
			(p->*doIt)();
        }
        else {
			if (policy == Component::ChildrenLast) {
				(p->*doIt)();
			}
			for (int i = 0; i < p->children().size(); ++i) {
				Component *child = dynamic_cast<Component*>(p->children().at(i));
				if (child) {
                    (child->*doItDeep)();
				}
			}
			if (policy == Component::ChildrenFirst) {
				(p->*doIt)();
			}
		}
	}
	
}

//! Initializes this and all children according to the RecursionPolicy.
void Component::deepInitialize()
{
    call(this, &Component::initialize, &Component::deepInitialize, recursionPolicy(Component::Initialize));
}

//! Resets this and all children according to the RecursionPolicy.
void Component::deepReset()
{
    call(this, &Component::reset, &Component::deepReset, recursionPolicy(Component::Reset));
}

//! Updates this and all children according to the RecursionPolicy.
void Component::deepUpdate()
{
    call(this, &Component::update, &Component::deepUpdate, recursionPolicy(Component::Update));
}

//! Cleans up this and all children according to the RecursionPolicy.
void Component::deepCleanup()
{
    call(this, &Component::cleanup, &Component::deepCleanup, recursionPolicy(Component::Cleanup));
}

//! Debriefs this this and all children according to the RecursionPolicy.
void Component::deepDebrief()
{
    call(this, &Component::debrief, &Component::deepDebrief, recursionPolicy(Component::Debrief));
}

//! Sets the RecursionPolicy
void Component::setRecursionPolicy(Function function, RecursionPolicy policy_)
{
    if (function == AllFunctions) {
        for (int f = Initialize; f < AllFunctions; ++f)
            policy[Function(f)] = policy_;
	}
	else {
        policy[function] = policy_;
	}
}	

//! Gets the recursion Policy
Component::RecursionPolicy Component::recursionPolicy(Function function) const
{
    Q_ASSERT(function != AllFunctions);
    return policy[function];
}

void Component::acceptPullVariableChanged(PullVariableBase *variable, ParameterBase *parameter) {
    parameter->setValueFromString(variable->toVariant().toString());
}


} //namespace

