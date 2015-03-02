/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "sliding_signal.h"

using namespace UniSim;

namespace vg {
	
/*! \class SlidingSignal
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

SlidingSignal::SlidingSignal(Identifier name, QObject *parent)
    : BaseSignal(name, parent)
{
    Input(double, input, 25.);
    Input(double, threshold, 25.);
    Input(double, thresholdBand, 5.);
    Input(double, minSignal, 0.);
    Input(double, maxSignal, 100.);
    Input(bool, increasingSignal, true);
}

double SlidingSignal::signal() {
    double res;
    double x0{threshold},
           x1{threshold + thresholdBand},
           y0{increasingSignal ? minSignal : maxSignal},
           y1{increasingSignal ? maxSignal : minSignal};
    if (input <= x0)
        res = y0;
    else if (input >= x1)
        res = y1;
    else {
        Q_ASSERT(thresholdBand != 0.);
        double gap = input - x0;
        res = y0 + slide(gap/thresholdBand)*(y1 - y0);
    }
    return res;
}

} //namespace

