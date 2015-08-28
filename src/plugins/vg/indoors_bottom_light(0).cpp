/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "indoors_bottom_light.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(IndoorsBottomLight)

/*! \class IndoorsBottomLight
 * \brief Computes indoors diffuse and direct light
 *
 * Inputs
 * ------
 * - _outdoorsDirectRadiation_ is the direct sunlight irradiation [W/m<SUP>2</SUP>]
 * - _outdoorsDiffuseRadiation_ is the diffuses sunlight irradiation [W/m<SUP>2</SUP>]
 * - _diffuseTransmission_ is the proportion of diffuse sunlight transmitted though the greenhouse surface [0;1]
 * - _directTransmissionAsDirect_ is the proportion of direct sunlight transmitted though the greenhouse cover and remaining surface [0;1]
 * - _directTransmissionAsDiffuse_ is the proportion of direct sunlight transmitted though the greenhouse surface and becoming dispersed as diffuse light [0;1]
 * - _growthLightShortWaveRadiation_ is the short wave radiation (light) emitted by the growth light [W/m<SUP>2</SUP>]
 * - _growthLightPropDirect_ is the proportion of growth light considered direct light [0;1]
 *
 * Outputs
 * ------
 * - _direct_ is the direct light transmitted directly through the greenhouse construction plus growth light [W/m<SUP>2</SUP>]
 * - _diffuse_ is the total diffuse light transmitted through and dispersed by the greenhouse construction [W/m<SUP>2</SUP>]
 * - _total_ is the total light transmitted through the greenhouse construction [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - an _outdoors_ model with two ports:
 *   + _directRadiation_ [W/m<SUP>2</SUP>]
 *   + _diffuseRadiation_ [W/m<SUP>2</SUP>]
 * - an _indoors/screens/transmission_ model with three ports:
 *   + _diffuse_ [0;1]
 *   + _directAsDirect_ [0;1]
 *   + _directAsDiffuse_ [0;1]
 * - an _actuators/growthLights_ model with a _shortWaveEmission_ port [W/m<SUP>2</SUP>]
 */

IndoorsBottomLight::IndoorsBottomLight(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, topDirect, "indoors/top/light[direct]");
    InputRef(double, topDiffuse, "indoors/top/light[diffuse]");
    InputRef(double, horizontalTransmissivity, "horizontalScreens[lightTransmissivity]");
    InputRef(double, growthLight, "actuators/growthLights[shortWaveEmission]");
    Input(double, growthLightPropDirect, 0.7);
    Output(double, direct);
    Output(double, diffuse);
    Output(double, total);
}

void IndoorsBottomLight::reset() {
    direct = diffuse = total = 0.;
}

void IndoorsBottomLight::update() {
    diffuse = horizontalTransmissivity*topDiffuse +
              (1.-growthLightPropDirect)*growthLight;
    direct = horizontalTransmissivity*topDirect +
             growthLightPropDirect*growthLight;
    total = direct + diffuse;
}


} //namespace

