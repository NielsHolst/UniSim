/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/test_num.h>
#include "base_control_element.h"

using namespace UniSim;

namespace vg {

/*! \class BaseControlElement
 * \brief Base class for control elements which maintain a _state_ in response to a _signal_
 *
 * Depending on the logic implemented in the pure virtual _change_ method, _state_ will approach
 * _signal_ with time
 *
 * Inputs
 * ------
 * - _initState_ is the initial value of state when control element is reset [any]
 * - _timeStepSecs_ is the integration time step [s]
 * - _signal_ is telling the desired state [any]
 *
 * Outputs
 * ------
 * - _state_ is the current state according to the time integration of _signal_ [any]
 * - _course_ is the current course of the signal [0 (decreasing), 1 (stable), 2 (increasing)]

 * Default dependencies
 * ------------
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 * - a parent model with a _signal_ port [any]
 */

BaseControlElement::BaseControlElement(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, initState, 0.);
    InputRef(double, timeStepSecs, "calendar[timeStepSecs]");
    InputRef(double, signal, "..[signal]");
    Output(double, state);
    Output(int, course);
}

void BaseControlElement::reset() {
    state = initState;
    course = Stable;
}

void BaseControlElement::update() {
    double oldState = state;
    state += change(signal - state);
    if (TestNum::eq(state, oldState))
        course = Stable;
    else
        course = (state < oldState) ? Decreasing : Increasing;
}

} //namespace
