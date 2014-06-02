/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "publish.h"
#include "ventilation_controller.h"
#include <usbase/utilities.h>

using namespace std;
using namespace UniSim;

namespace vg {
	
PUBLISH(VentilationController)

/*! \class VentilationController
 * \brief Calculates ventilation opening according to humidity, temperature and wind
 *
 * Inputs
 * ------
 * - _windSideProportion_ is the reduced opening in the windside, relative to the leeside [0;1]
 * - _frostThreshold_ is the threshold below which ventilation is always set to zero [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _byHumidity_ is the desired opening according to humidity [0;100]
 * - _byTemperature_ is the desired opening according to temperature [0;100]
 * - _byWind_ is the wind adjustment of the opening [0;1]
 * - _maxSignal_ is the maximum signal allowed [0;100]
 *
 * Outputs
 * ------
 * - _leeSideSignal_ is the desired ventilation opening in the lee side [0;_maxSignal_]
 * - _windSideSignal_ is the desired ventilation opening in the wind side[0;_maxSignal_]
 * - _signal_ is a synonym for _leeSideSignal_ [0;_maxSignal_]

 * Default dependencies
 * ------------
 * - an _outdoors_ model with a _temperature_ port [<SUP>o</SUP>C]
 * - an _setpoints/temperature/ventilation_ model with a _setpoint_ port [<SUP>o</SUP>C]
 * - a _byHumidity_ child model with a _signal_ port [0;100]
 * - a _byTemperature_ child model with a _signal_ port [0;100]
 * - a _byWind_ child model with a _signal_ port [0;100]
 * - a _maximum_ child model with a _signal_ port [0;100]
 */

VentilationController::VentilationController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, windSideProportion, 0.75);
    Input(double, frostThreshold, 4.);
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, byTemperature, "./byTemperature[signal]");
    InputRef(double, byHumidity, "./byHumidity[signal]");
//    InputRef(double, byWind, "./byWind[signal]");
    Input(double, byWind, 1.);
//    InputRef(double, maxSignal, "./maximum[value]");
    Input(double, maxSignal, 100.);
    Output(double, leeSideSignal);
    Output(double, windSideSignal);
    Output2(double, leeSideSignal, signal);
}

void VentilationController::reset() {
    leeSideSignal = windSideSignal = 0;
}

void VentilationController::update() {
    leeSideSignal = tooColdOutdoors()
                    ? 0
                    : max(byHumidity,
                          byTemperature)*byWind;
    leeSideSignal = min(leeSideSignal, maxSignal);
    windSideSignal = windSideProportion*leeSideSignal;
}

bool VentilationController::tooColdOutdoors() const {
    return outdoorsTemperature < frostThreshold;
}

} //namespace

