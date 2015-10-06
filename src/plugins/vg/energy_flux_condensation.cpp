/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_condensation.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxCondensation)

/*! \class EnergyFluxCondensation
 * \brief Flux of energy produced by condensation
 *
 * Input
 * ------
 * - _vapourFlux_ is the condensation rate per greenhouse ground area [kg/m<SUP>2</SUP>/s]
 */

EnergyFluxCondensation::EnergyFluxCondensation(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(double, vapourFlux, 0);
}

void EnergyFluxCondensation::update() {
    value = vapourFlux*LHe; // W/m2 = kg/m2/s * J/kg
}

} //namespace

