/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "energy_flux_ventilation.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxVentilation)

/*! \class EnergyFluxVentilation
 * \brief Flux of energy lost to ventilation
 *
 * Inputs
 * ------
 * - _ventilation_ is the rate of air exchange with the outdoors [h<SUP>-1</SUP>]
 * - _indoorsTemperature_ the ambient temperature indoors [<SUP>oC</SUP>C]
 * - _outdoorsTemperature_ the ambient temperature outdoors [<SUP>oC</SUP>C]
 * - _averageHeight_ is the average height of the greenhouse (volume divided by ground area) [m]
 */

EnergyFluxVentilation::EnergyFluxVentilation(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(double, ventilation, 0.);
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, averageHeight,"construction/geometry[averageHeight]");
}

void EnergyFluxVentilation::update() {
    double dT = outdoorsTemperature - indoorsTemperature;
    value = (dT < 0.) ? averageHeight*ventilation/3600*dT*CpAir*RhoAir : 0.; // W/m2 = m * h-1 / (s/h) * K * J/kg/K * kg/m3
}

} //namespace

