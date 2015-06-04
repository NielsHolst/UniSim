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
 * - _initState_ is the initial value of state when control element is reset [R]
 * - _timeStepSecs_ is the integration time step [s]
 * - _signal_ is telling the desired state [R]
 *
 * Outputs
 * ------
 * - _state_ is the current state according to the time integration of _signal_ [R]
 * - _value_ is synonumous with _state_ [R]
 * - _course_ is the current course of the signal [0 (decreasing), 1 (stable), 2 (increasing)]
 * - _fulfilment_ is the current _state_:_signal_ ratio [R]
 */

BaseControlElement::BaseControlElement(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, initState, 0.);
    InputRef(double, timeStepSecs, "calendar[timeStepSecs]");
    InputRef(double, signal, "..[signal]");
    Output(double, state);
    Output2(double, state, value);
    Output(double, fulfilment);
    Output(int, course);
}

void BaseControlElement::reset() {
    fulfilment = 1.;
    state = initState;
    course = Stable;
    localReset();
}

void BaseControlElement::update() {
    double oldState = state;
    state += change(signal - state);
    fulfilment = (signal==0.) ? 1. : state/signal;
    if (TestNum::eq(state, oldState))
        course = Stable;
    else
        course = (state < oldState) ? Decreasing : Increasing;
}

} //namespace
