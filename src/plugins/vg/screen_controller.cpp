/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "screen_controller.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(ScreenController)

/*! \class ScreenController
 * \brief This controller produces a signal [0;1] telling the desired screen position
 *
 * Inputs
 * ------
 * - _on_ tells whether screen be drawn or nor [true,false]
 * - _maxSignal_ sets an upper limit to the signal [0;1]
 *
 * Output
 * -----
 * - _signal_ is the desired blackout-screen position [0;_maxSignalAtHighHumidity_]
 *
 */

ScreenController::ScreenController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, followSignal, 0.);
    InputRef(bool, periodsOn, "./periods[on]");
    InputRef(bool, conditionsOn, "./conditions[on]");
    InputRef(double, suggestedSignal, "./suggestedSignal[value]");
    Output(double, signal);
}

void ScreenController::reset() {
    signal = 0;
}

void ScreenController::update() {
    signal = (periodsOn && conditionsOn) ? suggestedSignal : 0.;
    signal = max(signal, followSignal);
}

} //namespace
