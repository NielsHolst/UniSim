/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_ventilation.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxVentilation)

/*! \class EnergyFluxVentilation
 * \brief Flux of energy caused by  ventilation
 *
 * Inputs
 * ------
 * - _relativeVentilationRate_ is the relative exchange of air through ventilation during one time step [0;1]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _windspeed_ is the outdoors windspeed [m/s]
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _flux_ is the (mostly negative) energy flux per greenhouse area caused by ventilation [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _construction/geometry_ model with an _averageHeight_ port [m]
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 */

EnergyFluxVentilation::EnergyFluxVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, relativeVentilationRate, "indoors/ventilation[relative]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, averageHeight,"construction/geometry[averageHeight]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    Output(double, flux);
}

void EnergyFluxVentilation::reset() {
    flux = 0.;
}

void EnergyFluxVentilation::update() {
    // Partial integration of temperature change caused by ventilation alone
    double rate = relativeVentilationRate/timeStep,
           indoorsTemperatureFinal = propIntegral(indoorsTemperature, outdoorsTemperature, rate, timeStep);
    // Compute flux based on temperature change
    double Cair = averageHeight*RhoAir*CpAir; // J/m2/K = m * kg/m3 * J/kg/K
    flux = Cair*(indoorsTemperatureFinal-indoorsTemperature)/timeStep; // W/m2 = J/m2/K * K / s
}


} //namespace

