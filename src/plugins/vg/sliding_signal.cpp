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
* \brief A base class to control a signal according to the input
*
* The size of the _signal_ depends on where the _input_ is in relation to the interval [_threshold_; _threshold_+_thresholdBand_],
* and whether is is an _increasingSignal_.
* - An increasing signal goes from _minSignal_ for _input_ <  _threshold_, increasing to _maxSignal_ at _threshold_+_thresholdBand_ and above.
* - An decreasing signal goes from _maxSignal_ for _input_ <  _threshold_, decreasing to _minSignal_ at _threshold_+_thresholdBand_ and above.
*
* The exact course of the _signal_ in response to _input_ must be defined in a derived class, e.g. linear inside the thresholdBand for a
* ProportionalSignal.
*
* Inputs
* ------
* - _input_ is the value determining the signal [-]
* - _threshold_ is the inout threshold at which the signal begins changing [-]
* - _thresholdBand_ is the interval of the input during which the signal is changing [-]
* - _minSignal_ is the minimum possible signal [-]
* - _maxSignal_ is the maximum possible signal [-]
* - _increasingSignal_ tells whether the signal is increasing inside the threshold band [-]
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
    QString test = fullName();
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

