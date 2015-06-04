/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_short_wave.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxShortWave)

/*! \class EnergyFluxShortWave
 * \brief Flux of shortwave energy from sunlight and growth lights
 * Inputs
 * ------
 * - _indoorsRadiation_ is the total shortwave radiation indoors [W/m<SUP>2</SUP>]
 */

EnergyFluxShortWave::EnergyFluxShortWave(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    InputRef(double, indoorsRadiation, "indoors/radiation[total]");
}

void EnergyFluxShortWave::update() {
    value = indoorsRadiation;
}

} //namespace

