/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_light.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxLight)

/*! \class EnergyFluxLight
 * \brief Flux of shortwave energy from sunlight and growth lights
 * Inputs
 * ------
 * - _indoorsLight_ is the total shortwave radiation indoors [W/m<SUP>2</SUP>]
 */

EnergyFluxLight::EnergyFluxLight(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    InputRef(double, indoorsLight, "indoors/light[total]");
}

void EnergyFluxLight::update() {
    value = indoorsLight;
}

} //namespace

