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
 * Depending on the logic implemented in the _change_ method, _state_ will approach
 * _signal_ with time
 *
 * Inputs
 * ------
 * - _initState_ is the initial state when the control element is reset [-]
 * - _minimum_ is the minimum value that state can attain [-]
 * - _maximum_ is the maximum value that state can attain [-]
 * - _minSlope_ is the minimum slope that the rate of change in state can attain [min<SUP>-1</<SUP>]
 * - _maxSlope_ is the maximum slope that the rate of change in state can attain [min<SUP>-1</<SUP>]
 * - _timeStep_ is the integration time step [s]
 * - _signal_ is the desired state [-]
 *
 * Outputs
 * ------
 * - _state_ is the current state [-]
 * - _value_ is synonumous with _state_ [-]
 * - _slope_ is the rate of change in state [min<SUP>-1</<SUP>]
 */

BaseControlElement::BaseControlElement(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, initState, 0.);
    Input(double, minimum, -std::numeric_limits<double>::max());
    Input(double, maximum, std::numeric_limits<double>::max());
    Input(double, minSlope, -std::numeric_limits<double>::max());
    Input(double, maxSlope, std::numeric_limits<double>::max());
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, signal, "..[signal]");
    Output(double, state);
    Output2(double, state, value);
    Output(double, slope);
}

void BaseControlElement::reset() {
    state = state0 = state1 = initState;
    slope = 0;
    tick = 0;
    localReset();
}

void BaseControlElement::update() {
    state0 = state1;
    state1 = state2;
    state2 = state;
    state += change(signal - state);
    double dt = timeStep/60.;
    slope = fitSlopePPP(state1, state2, state)/dt;
    if (tick++>10) {
        if (slope < minSlope) {
            state = fitPointPPS(state1, state2, minSlope*dt);
            slope = minSlope;
        }
        else if (slope > maxSlope) {
            state = fitPointPPS(state1, state2, maxSlope*dt);
            slope = maxSlope;
        }
    }
    state = minMax(minimum, state, maximum);
}

} //namespace
