/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "pid_control_element.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(PidControlElement)

/*! \class PidControlElement
* \brief A base class to set a signal to approach a target value
*
* Proportional control provides a _signal_ to control the _actualValue_ towards the _targetValue_.
* The size of the _signal_ depends on the size of the gap between the actual and the target value.
* When the gap is in the range of the proportional bad [0;_pBand_], the signal is proportional to the gap.
* When the gap is larger than _pBand_ the signal is equal to the _maxSignal_.
*
* In the figures below, the blue curves represent ProportionalControl and the red curves AsymptoticControl.
* When _direction_ is set to "floor" (left figure below), the target is considered a desired minimum value.
* Below the target value, the signal wil be zero.
*
* When _direction_ is set to "ceiling" (right figure below), the target is considered a desired maximum value.
* Above the target value, the signal wil be zero.
*
* ![Caption text](/data/QDev/UniSim/doc/images/proportional_control.png)
*
* Inputs
* ------
* - _input_ is the current value which should be controlled towards the _targetValue_ [R]
* - _threshold_ is that target value that the controller is aiming towards [R]
* - _thresholdBand_ is the sliding band of the control. Negative or positive whether it lies below or above the _threshold_ [R]
* - _direction_ sets the direction of control: upwards or downwords ["ceiling", "floor"]
* - _signalOutsideBand_ is the signal beyound the _thresholdBand_ [R]
* - _signalAtThreshold_ is the signal at the _threshold_ and beyond [R]
*
*/

PidControlElement::PidControlElement(Identifier name, QObject *parent)
    : BaseControlElement(name, parent)
{
    Input(double, Kprop, 0.5);
    Input(double, Kint, 0.);
    Input(double, Kderiv, 0.);
    Output(double, errorIntegral);
    Output(double, errorDerivative);
}

void PidControlElement::localReset() {
    errorIntegral = 0.;
    firstChange = true;
}

double PidControlElement::change(double error) {
    errorDerivative = firstChange ? 0. : (error - prevError); // /timeStepSecs;
    prevError = error;
    firstChange = false;
    errorIntegral += error; //*timeStepSecs;
    return Kprop*error + Kint*errorIntegral + Kderiv*errorDerivative;
}

} //namespace

