/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/test_num.h>
#include "control_element_base.h"

using namespace UniSim;

namespace vg {
	
ControlElementBase::ControlElementBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(signal), "..[signal]");
    addParameterRef<double>(Name(timeStepSecs), "calendar[timeStepSecs]");
    addParameter<double>(Name(initState), 0. , "Initial value of @F {state}");
    addVariable<double>(Name(state),"Current state of control element");
    addVariable<int>(Name(course),"Current course of change: @F {Decreasing}, @F Stable or @F {Increasing}");
}

void ControlElementBase::reset() {
    state = initState;
    course = Stable;
}

void ControlElementBase::update() {
    double oldState = state;
    state += change(signal - state);
    if (TestNum::eq(state, oldState))
        course = Stable;
    else
        course = (state < oldState) ? Decreasing : Increasing;
}

} //namespace
