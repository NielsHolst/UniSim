/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_growth_lights.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxGrowthLights)

/*! \class EnergyFluxGrowthLights
 * \brief Flux of energy from growth lights
 * Inputs
 * ------
 * - _energyFlux_ is the total energy emitted by the growth lights [W/m<SUP>2</SUP>]
 */

EnergyFluxGrowthLights::EnergyFluxGrowthLights(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    InputRef(double, energyFlux, "actuators/growthlights[energyFlux]");
}

void EnergyFluxGrowthLights::update() {
    value = energyFlux;
}

} //namespace

