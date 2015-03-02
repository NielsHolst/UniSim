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
 * - _byHumidity_ is the desired opening according to humidity [0;100]
 * - _byTemperature_ is the desired opening according to temperature [0;100]
 * - _leeFactor_ is a leeward correction factor for windspeed [0;1]
 * - _windFactor_ is a windwad correction factor for windspeed [0;1]
 * - _coldFactor_ is a correction factor for outdoors cold [0;1]
 *
 * Outputs
 * ------
 * - _leewardSignal_ is the desired ventilation opening in the lee side [0;100]
 * - _windwardSignal_ is the desired ventilation opening in the wind side [0;100]

 * Default dependencies
 * ------------
 * - a _coldProtection_ child model with a _flag_ port [0;100]
 * - a _byHumidity_ child model with a _signal_ port [0;100]
 * - a _byTemperature_ child model with a _signal_ port [0;100]
 */

VentilationController::VentilationController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, byTemperature, "./byTemperature[signal]");
    InputRef(double, byHumidity, "./byHumidity[signal]");
    InputRef(double, leeFactor, "./leeFactor[signal]");
    InputRef(double, windFactor, "./windFactor[signal]");
    InputRef(double, coldFactor, "./coldFactor[signal]");
    Output(double, leewardSignal);
    Output(double, windwardSignal);
}

void VentilationController::reset() {
    leewardSignal = windwardSignal = 0;
}

void VentilationController::update() {
    double signal = max(byHumidity, byTemperature)*coldFactor;
    leewardSignal = signal*leeFactor;
    windwardSignal = signal*windFactor;
}

} //namespace

