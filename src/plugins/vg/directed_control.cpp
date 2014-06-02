/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "directed_control.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(DirectedControl)

/*! \class DirectedControl
* \brief A base class to provide control towards a target value
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
* - _actualValue_ is the current value which should be controlled towards the _targetValue_ [R]
* - _targetValue_ is that target value that the controller is aiming towards [R]
* - _pBand_ is the proportional band of the control [R]
* - _direction_ sets the direction of control: upwards or downwords ["ceiling", "floor"]
* - _minSignal_ is the maximum of the resulting signal [R]
* - _maxSignal_ is the maximum of the resulting signal [R]
*
* Outputs
* -------
* - _signal_ is the signal [_minSignal_;_maxSignal_] which can be used to control the actual value towards the target value [R]
*
* Default dependencies
* ------------
* - none
*/

DirectedControl::DirectedControl(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, actualValue, 25.);
    Input(double, targetValue, 25.);
    Input(double, pBand, 5.);
    Input2(QString, strDirection, direction, "ceiling");
    Input(double, minSignal, 0.);
    Input(double, maxSignal, 100.);
    Output(double, signal);

    directions["floor"] = Floor;
    directions["ceiling"] = Ceiling;
}

void DirectedControl::initialize() {
    _direction = directions.seek(strDirection, this);
}

void DirectedControl::reset() {
    signal = 0;
}

} //namespace

