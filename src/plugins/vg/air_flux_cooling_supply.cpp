/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "air_flux_cooling_supply.h"

using namespace UniSim;
using std::min;

namespace vg {

PUBLISH(AirFluxCoolingSupply)

/*! \class AirFluxCoolingSupply
 * \brief Total air ventilation
 *
 * Inputs
 * ------
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _active_ is the active ventilation [h<SUP>-1</SUP>]
 *
 * Outputs
 * ------
 * - _value_ is the relative rate of air exchanged [h<SUP>-1</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _construction/geometry_ model with an _averageHeight_ port [m]
 */

AirFluxCoolingSupply::AirFluxCoolingSupply(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, energyFlux, "energyFlux/cooling/supply[state]");
    InputRef(double, airSupplyMax, "cooling/airSupplyMax[value]");
    InputRef(double, bottomHeight, "construction/geometry[height]");
    InputRef(double, topHeight, "construction/geometry[roofAverageHeight]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    Output(double, fromOutdoorsToIndoors);
    Output(double, fromOutdoorsToTop);
}

void AirFluxCoolingSupply::reset() {
    fromOutdoorsToIndoors = fromOutdoorsToTop = 0;
}

void AirFluxCoolingSupply::update() {
    double dT = outdoorsTemperature - indoorsTemperature;
    if (dT < 0.) {
        // W*h/m2/K = m * J/kg/K * kg/m3 * h/s
        double C = bottomHeight*CpAir*RhoAir/3600;
        // h-1 = W/m2 / (W*h/m2/K) / K = W/m2 * K*m2/(W*h) / K
        fromOutdoorsToIndoors = min(energyFlux/C/dT, airSupplyMax);
        Q_ASSERT(fromOutdoorsToIndoors >= 0.);
        fromOutdoorsToTop = fromOutdoorsToIndoors;
    }
    else {
        fromOutdoorsToIndoors = fromOutdoorsToTop = 0;
    }
}

//void VentilationForCoolingBottom::update() {
//    double dToutdoors = gap*(outdoorsTemperature - indoorsTemperature),
//           dTtop = (1-gap)*(topTemperature - indoorsTemperature);
//    // W*h/m2/K = m * J/kg/K * kg/m3 * h/s
//    double C = averageHeight*CpAir*RhoAir/3600;
//    // h-1 = W/m2 / (W*h/m2/K) / K = W/m2 * K*m2/(W*h) / K
//    fromOutdoors = energyFlux/C/dToutdoors;
//    fromTop = energyFlux/C/dTtop;
//}

} //namespace

