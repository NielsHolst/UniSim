/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "ventilation_by_wind.h"
#include "general.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(VentilationByWind)

/*! \class VentilationByWind
 * \brief Flux of energy needed to cool down to ventilation setpoint
 * Inputs
 * ------
 * - _passiveTemperature_ is the indoors tempeature reached, it not actively regulated [<SUP>o</SUP>C]
 * - _heatingSetpoint_ is the heating setpoint [<SUP>oC</SUP>C]
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _value_ is the energy flux needed to heat the greenhouse to the heating setpoint [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - an _passive/temperature_ model with a _value_ port [<SUP>oC</SUP>C]
 * - a _setpoints/temperature/heating_ model with a _signal_ port [<SUP>oC</SUP>C]
 * - a _construction/geometry_ model with an _averageHeight_ port [m]
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 */

VentilationByWind::VentilationByWind(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, baseRate, 30.);
    InputRef(double, windspeed, "outdoors[windspeed]");
    InputRef(double, ventsProportionalEffectiveArea, "construction/vents[proportionalEffectiveArea]");
    Output(double, value);
}

void VentilationByWind::reset() {
    value = baseRate;
}

void VentilationByWind::update() {
    value = baseRate*windspeed*ventsProportionalEffectiveArea;
}

} //namespace

