/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "cover.h"
#include "general.h"
#include "publish.h"
#include "screens.h"
#include "shelter_base.h"

using namespace UniSim;

namespace vg {


/*! \class ShelterBase
 * \brief Common outputs for Shelter and Shelters
 *
 * Outputs
 * ------
 * - _diffuseLightTransmitted_ is the intensity of sunlight transmitted as diffuse light [W/m<SUP>2</SUP> ground]
 * - _directLightTransmitted_ is the intensity of sunlight transmitted as direct light [W/m<SUP>2</SUP> ground]
 * - _totalLightTransmitted_ is the total intensity of sunlight transmitted (diffuse+direct) [W/m<SUP>2</SUP> ground]
 * - _lightAbsorbedCover_ is the intensity of sunlight absorbed by the cover [W/m<SUP>2</SUP> ground]
 * - _lightAbsorbedScreens_ is the intensity of sunlight absorbed by the screens [W/m<SUP>2</SUP> ground]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the cover and screens [0;1]
 * - _U_ is the combined U-value of cover and screens [W/m<SUP>2</SUP>/K]
 * - _airTransmissivity_ is the proportion of air transmitted through the cover and screens [0;1]
 */

ShelterBase::ShelterBase(Identifier name, QObject *parent)
    : SurfaceRadiationOutputs(name, parent)
{
    Output(double, diffuseLightTransmitted);
    Output(double, directLightTransmitted);
    Output(double, totalLightTransmitted);
    Output(double, lightAbsorbedCover);
    Output(double, lightAbsorbedScreens);
    Output(double, haze);
    Output(double, U);
    Output(double, airTransmissivity);
}


void ShelterBase::reset() {
    resetRadiationOutputs();
    diffuseLightTransmitted = directLightTransmitted = totalLightTransmitted =
    lightAbsorbedCover = lightAbsorbedScreens =
    haze = U = 0.;
    airTransmissivity = 1.;
}


} //namespace

