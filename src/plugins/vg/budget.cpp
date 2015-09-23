/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "budget.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(Budget)

/*! \class Budget
 * \brief
 *
 * Inputs
 * ------
 *
 * Output
 * ------
 */

Budget::Budget(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, heatingEnergyFlux, "heating/supply[value]");
    InputRef(double, growthLightsEnergyFlux, "actuators/growthLights[energyFlux]");
    InputRef(double, co2Flux, "controllers/co2[signal]");
    InputRef(double, dt, "calendar[timeStepSecs]");
    Output(double, heatingEnergyTotal);
    Output(double, growthLightsEnergyTotal);
    Output(double, co2Total);
}

void Budget::reset() {
    heatingEnergyFlux = heatingEnergyTotal =
    growthLightsEnergyFlux = growthLightsEnergyTotal =
    co2Flux = co2Total = 0.;
}

void Budget::update() {
    heatingEnergyTotal += heatingEnergyFlux*dt/3600./1000.;             // kWh/m2
    growthLightsEnergyTotal += growthLightsEnergyFlux*dt/3600./1000.;   // kWh/m2
    co2Total += co2Flux*dt/3600./1000.;                                 // kg/m2
}


} //namespace

