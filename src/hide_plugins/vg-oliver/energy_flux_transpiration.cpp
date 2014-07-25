/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_transpiration.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxTranspiration)

/*! \class EnergyFluxSurface
 * \brief Flux of energy lost to transpiration
 * Inputs
 * ------
 * - _transpiration_ is the plant transpiration rate [kg/m<SUP>2</SUP>/s]
 *
 * Output
 * ------
 * - _flux_ is the flux of energy lost to transpiration [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - *pending*
 */

EnergyFluxTranspiration::EnergyFluxTranspiration(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, transpiration, "transpiration/crop[vapourFlux]");
    Output(double, flux);
}

void EnergyFluxTranspiration::reset() {
    flux = 0.;
}

void EnergyFluxTranspiration::update() {
    flux = -transpiration * LHe; // kg/m2/s * J/kg
}

} //namespace

