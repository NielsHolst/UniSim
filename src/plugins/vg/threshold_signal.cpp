/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "threshold_signal.h"

using namespace UniSim;

namespace vg {

PUBLISH(ThresholdSignal)

/*! \class ThresholdSignal
 * \brief Sets a signal according to a threshold value
 *
 * Inputs
 * ------
 * - _input_ is the value determining the signal [-]
 * - _threshold_ is the input threshold [-]
 * - _signalBelow_ is the resulting _signal_ when _input_ < _threshold_ [-]
 * - _signalAbove_ is the resulting _signal_ when _input_ >= _threshold_ [-]
 *
 */

ThresholdSignal::ThresholdSignal(Identifier name, QObject *parent)
    : BaseSignal(name, parent){
    Input(double, threshold, 0.);
    Input(double, input, 0.);
    Input(double, signalBelow, 0.);
    Input(double, signalAbove, 1.);
}

double ThresholdSignal::signal() {
    return (input < threshold) ? signalBelow : signalAbove;
}

} //namespace

