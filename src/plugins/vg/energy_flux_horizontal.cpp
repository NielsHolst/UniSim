/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "energy_flux_horizontal.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxHorizontal)

/*! \class EnergyFluxHorizontal
 * \brief Flux of energy carried by air flux
 *
 * Inputs
 * ------
 * - _airFlux_ is the rate of air exchange between compartments [h<SUP>-1</SUP>]
 * - _receiverTemperature_ is the air temperature of the receiving compartment [<SUP>oC</SUP>C]
 * - _outdoorsTemperature_ is the air temperature of the donor compartment [<SUP>oC</SUP>C]
 * - _averageHeight_ is the average height of the receiving compartment [m]
 */

EnergyFluxHorizontal::EnergyFluxHorizontal(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(double, U, 2.8);
    Input(double, receiverTemperature, 0.);
    Input(double, donorTemperature, 0.);
    InputRef(double, state, "horizontalScreens[maxState]");
}

void EnergyFluxHorizontal::update() {
    double dT = donorTemperature - receiverTemperature;
    value = U*dT*state;     // W/m2 = W/m2/K * K * 1
}

} //namespace

