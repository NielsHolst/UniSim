/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_temperature.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(IndoorsTemperature)

/*! \class PassiveTemperature
 * \brief Indoors temperature if not actively regulated
 *
 * Inputs
 * ------
 * - _initValue_ is the initial indoors temperature [<SUP>o</SUP>C]
 * - _indoorsTemperature_ is the current indoors temperature [<SUP>o</SUP>C]
 * - _energyFlux_ is the summed passive energy fluxes in the greenhouse [W/m<SUP>2</SUP>]
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _value_ is the indoors tempeature reached, it not actively regulated [<SUP>o</SUP>C]
 *
 * Default dependencies
 * ------------
 * - a _construction/geometry_ model with an _averageHeight_ port [m]
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 */

IndoorsTemperature::IndoorsTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, initValue, 20.);
    InputRef(double, baseTemperature, ".[value]");
    Input(double, energyFlux, 0.);
    InputRef(double, averageHeight,"construction/geometry[averageHeight]");
    InputRef(double, timeStep,"calendar[timeStepSecs]");
    Output(double, value);
    Output(double, change);
}


void IndoorsTemperature::reset() {
    value = initValue;
    change = 0.;
    tick = 0;
}

void IndoorsTemperature::update() {
    // Keep temperature constant for the first few time steps to stabilise overall model state
    if (tick++ < 10) return;
    double Cair = averageHeight*RhoAir*CpAir;               // J/m2/K = m * kg/m3 * J/kg/K
    change = energyFlux*timeStep/Cair;  // K = W/m2 * s / (J/m2/K)
    value = baseTemperature + change;
}

} //namespace
