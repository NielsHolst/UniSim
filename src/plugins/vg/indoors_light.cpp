/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "indoors_light.h"
#include "publish.h"
#include "shelter.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(IndoorsLight)

/*! \class IndoorsLight
 * \brief Indoors diffuse and direct, light and PAR
 *
 * Indoors light is the combination of incoming sunlight and growth lights
 *
 * Inputs
 * ------
 * - _sunlightDiffuse_ is the intensity of diffuse light resulting from transmitted sunlight [W/m<SUP>2</SUP>]
 * - _sunlightDirect_ is the intensity of direct light resulting from transmitted sunlight [W/m<SUP>2</SUP>]
 * - _sunlightPropPar_ is the proportion of PAR in sunlight irradiation [0;1]
 * - _growthLigthtsDirect_ is the intensity of direct light supplied by growth lights [W/m<SUP>2</SUP>]
 * - _growthLigthtsPar_ is the intensity of PAR supplied by growth lights [W/m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _direct_ is the intensity of direct light indoors [W/m<SUP>2</SUP>]
 * - _diffuse_ is the intensity of diffuse light indoors [W/m<SUP>2</SUP>]
 * - _total_ is total the intensity of light indoors [W/m<SUP>2</SUP>]
 * - _parDirect_ is the intensity of direct PAR indoors [W/m<SUP>2</SUP>]
 * - _parDiffuse_ is the intensity of diffuse PAR indoors [W/m<SUP>2</SUP>]
 * - _parTotal_ is the total intensity of PAR indoors [W/m<SUP>2</SUP>]
 *
 */

IndoorsLight::IndoorsLight(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, sunlightDiffuse, "construction/shelters[diffuseLightTransmitted]");
    InputRef(double, sunlightDirect, "construction/shelters[directLightTransmitted]");
    InputRef(double, sunlightPropPar, "outdoors[propParRadiation]");
    InputRef(double, growthLigthtsDirect, "growthLights[shortWaveEmission]");
    InputRef(double, growthLigthtsPar, "growthLights[parEmission]");
    Output(double, direct);
    Output(double, diffuse);
    Output(double, total);
    Output(double, parDirect);
    Output(double, parDiffuse);
    Output(double, parTotal);
}

void IndoorsLight::reset() {
    diffuse = direct = total =
    parDiffuse = parDirect = parTotal = 0.;
}

void IndoorsLight::update() {
    diffuse = sunlightDiffuse;
    direct = sunlightDirect + growthLigthtsDirect;
    total = direct + diffuse;

    parDiffuse = sunlightPropPar*diffuse;
    parDirect = sunlightPropPar*direct + growthLigthtsPar;
    parTotal = parDiffuse + parDirect;

}


} //namespace

