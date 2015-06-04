/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "energy_flux_cooling_supply.h"
#include "general.h"
#include "publish.h"

using std::min;
using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxCoolingSupply)

/*! \class EnergyFluxCoolingSupply
 * \brief Flux of energy supplied to cool the greenhouse down towards the ventilation setpoint
 * Inputs
 * ------
 * - _ventilationDemand_ is the flux of energy demanded to cool the greenhouse down to the ventilation setpoint [W/m<SUP>2</SUP>]
 * - _byWind_ is the air exchange rate that can be caused by outside wind alone [h<SUP>-1</SUP>]
 * - _byTemp_ is the air exchange rate that can be caused by the outside-inside temperature difference alone [h<SUP>-1</SUP>]
 * - _indoorsTemperature_ is the indoors temperature [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the outdoors temperature [<SUP>o</SUP>C]
 * - _averageHeight_ is the average height of the greenhouse [m]
 *
 * Output
 * ------
 * - _value_ is the flux of energy supplied to cool the greenhouse down towards the ventilation setpoint [W/m<SUP>2</SUP>]
 * - _airFlux_ is the air flux that causes the energy flux [h<SUP>-1</SUP>]
 */

EnergyFluxCoolingSupply::EnergyFluxCoolingSupply(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, ventilationDemand, "cooling/demand[value]");
    InputRef(double, byWind, "./byWind[value]");
    InputRef(double, byTemp, "./byTemp[value]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, averageHeight,"construction/geometry[averageHeight]");
    Output(double, value);
    Output(double, airFlux);
}

void EnergyFluxCoolingSupply::reset() {
    value = airFlux = 0.;
}

void EnergyFluxCoolingSupply::update() {
    double dT = outdoorsTemperature - indoorsTemperature;
    if (dT < 0.) {
        double maxVentilationSupply = sqrt(sqr(byWind) + sqr(byTemp)), // h-1
               // W/m2 = m * h-1 / (s/h) * K * J/kg/K * kg/m3
               maxEnergySupply = averageHeight*maxVentilationSupply/3600*dT*CpAir*RhoAir;
        value = max(ventilationDemand, maxEnergySupply); // both are negative or zero
    }
    else {
        value = 0.;
    }
    // h-1 = W/m2 / m / K / (J/kg/K) / (kg/m3) * s/h
    airFlux = value/averageHeight/dT/CpAir/RhoAir*3600;
}

} //namespace

