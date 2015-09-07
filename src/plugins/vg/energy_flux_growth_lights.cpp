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
 * - _heatEmission_ is the convective heat emission [W/m<SUP>2</SUP>]
 * - _longWaveEmission_ is the long wave emission (thermal radiation, Bn) [W/m<SUP>2</SUP>]
 *
 * Output
 * ------
 * - _value_ is the energy flux per greenhouse area from the growth lights [W/m<SUP>2</SUP>]
 */

EnergyFluxGrowthLights::EnergyFluxGrowthLights(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    InputRef(double, heatEmission, "actuators/growthlights[heatEmission]");
}

void EnergyFluxGrowthLights::update() {
    value = heatEmission;
}

} //namespace

