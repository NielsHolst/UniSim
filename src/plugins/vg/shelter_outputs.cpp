/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "shelter_outputs.h"

using namespace UniSim;

namespace vg {

/*! \class ShelterOutputs
 * \brief Common shelter outputs
 *
 * Outputs
 * ------
 * - _U_ is the heat transfer coefficient of the shelter [W/K/m<SUP>2</SUP>]
 * - _airTransmission_ is the proportion of air transmitted through the shelter [0;1]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the shelter [0;1]
 * - _diffuseLightTransmission_ is the proportion of diffuse light transmitted through the shelter [0;1]
 * - _directLightTransmissionAsDirect_ is the proportion of direct light transmitted through the shelter as direct light [0;1]
 * - _directLightTransmissionAsDiffuse_ is the proportion of direct light transmitted through the shelter as diffuse light [0;1]
 */

ShelterOutputs::ShelterOutputs(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(double, U);
    Output(double, airTransmission);
    Output(double, haze);
    Output(double, diffuseLightTransmission);
    Output(double, directLightTransmissionAsDirect);
    Output(double, directLightTransmissionAsDiffuse);
}

void ShelterOutputs::reset() {
    U = 0;
    airTransmission = 1.;
    haze = 0.;
    diffuseLightTransmission =
    directLightTransmissionAsDirect =
    directLightTransmissionAsDiffuse = 1.;
    localReset();
}

} //namespace

