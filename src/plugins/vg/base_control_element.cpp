/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <limits>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "base_control_element.h"
#include "general.h"

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
 * - _rateOfChange_ is the rate of change in _state_ during this time step [s<SUP>-1</SUP>]
 * - _course_ is the current course of the signal [0 (decreasing), 1 (stable), 2 (increasing)]
 * - _fulfilment_ is the current _state_:_signal_ ratio [R]
 */

BaseControlElement::BaseControlElement(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, initState, 0.);
    Input(double, minimum, -std::numeric_limits<double>::max());
    Input(double, maximum, std::numeric_limits<double>::max());
    Input(double, minSlope, -std::numeric_limits<double>::max());
    Input(double, maxSlope, std::numeric_limits<double>::max());
    InputRef(double, timeStepSecs, "calendar[timeStepSecs]");
    InputRef(double, signal, "..[signal]");
    Output(double, state);
    Output2(double, state, value);
    Output(double, slope);
    Output(double, predicted);
}

void BaseControlElement::reset() {
    state = state0 = state1 = predicted = initState;
    slope = 0;
    tick = 0;
    localReset();
}

void BaseControlElement::update() {
    state0 = state1;
    state1 = state2;
    state2 = state;
    state += change(signal - state);
    state = minMax(minimum, state, maximum);
    slope = fitSlopePPP(state1, state2, state);
    if (tick++>10) {
        if (slope < minSlope) {
            state = fitPointPPS(state1, state2, minSlope);
            slope = minSlope;
        }
        else if (slope > maxSlope) {
            state = fitPointPPS(state1, state2, maxSlope);
            slope = maxSlope;
        }
    }
}

} //namespace
