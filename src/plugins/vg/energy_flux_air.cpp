/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "energy_flux_air.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxAir)

/*! \class EnergyFluxAir
 * \brief Flux of energy carried by air flux
 *
 * Inputs
 * ------
 * - _airFlux_ is the rate of air exchange between compartments [h<SUP>-1</SUP>]
 * - _receiverTemperature_ is the air temperature of the receiving compartment [<SUP>oC</SUP>C]
 * - _outdoorsTemperature_ is the air temperature of the donor compartment [<SUP>oC</SUP>C]
 * - _averageHeight_ is the average height of the receiving compartment [m]
 */

EnergyFluxAir::EnergyFluxAir(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(double, airFlux, 0.);
    InputRef(double, receiverTemperature, "indoors/temperature[value]");
    InputRef(double, donorTemperature, "outdoors[temperature]");
    InputRef(double, averageHeight,"construction/geometry[averageHeight]");
}

void EnergyFluxAir::update() {
    double dT = donorTemperature - receiverTemperature;
    value = averageHeight*airFlux/3600*dT*CpAir*RhoAir; // W/m2 = m * h-1 / (s/h) * K * J/kg/K * kg/m3
//    value = (dT < 0.) ? averageHeight*ventilation/3600*dT*CpAir*RhoAir : 0.;
}

} //namespace

