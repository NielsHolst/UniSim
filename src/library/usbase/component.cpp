/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <qmessagebox.h>
#include <QString>
#include "component.h"
#include "exception.h"
#include "parameter_base.h"
#include "utilities.h"

namespace UniSim{

Component::Component(Identifier name, QObject *parent)
    : NamedObject(name, parent)
{
    Input(int, runSteps, 1);
    Input(int, runIterations, 1);
    Output(int, runStep);
    Output(int, runIteration);
    setRecursionPolicy(Component::AllFunctions, Component::ChildrenFirst);
}

namespace {
	
	typedef void (Component::*FunctionPtr)();
	
    void call(Component *p, FunctionPtr doIt, FunctionPtr doItDeep, Component::RecursionPolicy policy)
	{
		if (policy == Component::ChildrenNot) {
            p->followRedirections();
			(p->*doIt)();
        }
        else {
			if (policy == Component::ChildrenLast) {
                p->followRedirections();
                (p->*doIt)();
			}
			for (int i = 0; i < p->children().size(); ++i) {
				Component *child = dynamic_cast<Component*>(p->children().at(i));
				if (child) {
                    (child->*doItDeep)();
				}
			}
			if (policy == Component::ChildrenFirst) {
                p->followRedirections();
                (p->*doIt)();
			}
		}
	}
	
}

void Component::followRedirections() {
    for (int i = 0; i < parameters.size(); ++i)
        parameters[i]->followRedirection();
}

void Component::resolveReferences() {
    for (auto var : seekChildren<VariableBase*>("*"))
        var->resolveReference();
    for (auto comp : seekChildren<Component*>("*"))
        comp->resolveReferences();
}

//! Amends this and all children according to the RecursionPolicy.
void Component::deepAmend() {
    parameters = seekChildren<ParameterBase*>("*");
    call(this, &Component::amend, &Component::deepAmend, recursionPolicy(Component::Amend));
}

//! Initializes this and all children according to the RecursionPolicy.
void Component::deepInitialize() {
    auto before= seekDescendants<NamedObject*>("*");
    call(this, &Component::initialize, &Component::deepInitialize, recursionPolicy(Component::Initialize));
    auto after = seekDescendants<NamedObject*>("*");

    if (before.size() != after.size()) {
        QString msg("It is illegal to create children in initialize(). "
                    "Descendant count before/after deepInitialize: %1/%2. "
                    "Additional children must be created in constructor or amend().\n");
        msg += "Before: ";
        for (int i = 0; i < before.size(); ++i)
            msg += before.at(i)->objectName() + ", ";
        msg += "\nAfter: ";
        for (int i = 0; i < after.size(); ++i)
            msg += after.at(i)->objectName() + ", ";
        throw Exception(msg.arg(before.size()).arg(after.size()));
    }
}

//! Resets this and all children according to the RecursionPolicy.
void Component::deepReset() {
    call(this, &Component::reset, &Component::deepReset, recursionPolicy(Component::Reset));
}

//! Updates this and all children according to the RecursionPolicy.
void Component::deepUpdate() {
    call(this, &Component::update, &Component::deepUpdate, recursionPolicy(Component::Update));
}

//! Cleans up this and all children according to the RecursionPolicy.
void Component::deepCleanup() {
    call(this, &Component::cleanup, &Component::deepCleanup, recursionPolicy(Component::Cleanup));
}

//! Debriefs this this and all children according to the RecursionPolicy.
void Component::deepDebrief() {
    call(this, &Component::debrief, &Component::deepDebrief, recursionPolicy(Component::Debrief));
}

//! Sets the RecursionPolicy
void Component::setRecursionPolicy(Function function, RecursionPolicy policy_) {
    if (function == AllFunctions) {
        for (int f = Initialize; f < AllFunctions; ++f)
            policy[Function(f)] = policy_;
	}
	else {
        policy[function] = policy_;
	}
}	

//! Gets the recursion Policy
Component::RecursionPolicy Component::recursionPolicy(Function function) const {
    Q_ASSERT(function != AllFunctions);
    return policy[function];
}

void Component::run() {
    runIteration = 0;
    deepInitialize();
    while (runIteration < runIterations) {
        ++runIteration;
        runStep = 0;
        deepReset();
        while (runStep < runSteps) {
            ++runStep;
            deepUpdate();
        }
        deepCleanup();
    }
    deepDebrief();
}

} //namespace

