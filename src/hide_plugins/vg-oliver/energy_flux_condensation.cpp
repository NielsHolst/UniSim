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
 * Output
 * ------
 * - _flux_ is the flux of energy produced by condensation [W/m<SUP>2</SUP>]
 *
 * Optional dependencies
 * ------------
 * - some (0..n) child models of _humidity/condensation_ with a _vapourFlux_ port [kg/m<SUP>2</SUP>/s]
 */

EnergyFluxCondensation::EnergyFluxCondensation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(double, flux);
}

void EnergyFluxCondensation::initialize() {
    fluxes.clear();
    auto fluxModels = seekMany<Model*>("humidity/condensation/*");
    for (auto model : fluxModels)
        fluxes << model->pullValuePtr<double>("vapourFlux");
}

void EnergyFluxCondensation::reset() {
    flux = 0.;
}

void EnergyFluxCondensation::update() {
    double sum{0};
    for (auto fl : fluxes)
        sum += *fl;
    flux = sum*LHe; // kg/m2/s * J/kg
}

} //namespace

