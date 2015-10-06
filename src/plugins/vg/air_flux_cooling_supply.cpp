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
 * \brief Flux of air from the outdoors supplied for greenhouse ventilation
 * Computed from the corresponding supplied cooling energy obtained by ventilation.
 *
 * Inputs
 * ------
 * - _energyFlux_ is the supplied cooling energy [W/m<SUP>2</SUP>]
 * - _airSupplyMax_ is the maximum possible air flux [h<SUP>-1</SUP>]
 * - _indoorsHeight_ is the average height of the greenhouse (volume divided by ground area) [m]
 * - _indoorsTemperature_ is the indoors tempeature [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is ambient temperature outdoors [<SUP>o</SUP>C]
 *
 * Outputs
 * ------
 * - _value_ is the relative rate of air exchanged [h<SUP>-1</SUP>]
 *
 */

AirFluxCoolingSupply::AirFluxCoolingSupply(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, energyFlux, "energyFlux/cooling/supply[value]");
    InputRef(double, airSupplyMax, "cooling/airSupplyMax[value]");
    InputRef(double, indoorsHeight, "geometry[indoorsAverageHeight]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    Output(double, value);
}

void AirFluxCoolingSupply::reset() {
    value =  0;
}

void AirFluxCoolingSupply::update() {
    double dT = outdoorsTemperature - indoorsTemperature;
    if (dT < 0.) {
        // W*h/m2/K = m * J/kg/K * kg/m3 * h/s
        double C = indoorsHeight*CpAir*RhoAir/3600;
        // h-1 = W/m2 / (W*h/m2/K) / K = W/m2 * K*m2/(W*h) / K
        value = min(energyFlux/C/dT, airSupplyMax);
        Q_ASSERT(value >= 0.);
    }
    else {
        value = 0;
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

