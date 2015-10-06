/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "crop_radiation.h"
#include "general.h"
#include "publish.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(CropRadiation)


/*! \class CropRadiation
 * \brief Basic parameters for radiation in crop canopy
 *
 * Inputs
 * ------
 * - _kDiffuse_ is the extinction coefficient for diffuse light [0;1]
 * - _scattering_ is the scattering coefficent for direct light [0;1]
 * - _sinb_ is the sine of sun elevation [-1;1]
 * - _lightDiffuse_ is the intensity of diffuse light indoors [W/m<SUP>2</SUP>]
 * - _lightDirect_ is the intensity of direct light indoors [W/m<SUP>2</SUP>]
 * - _absorptivityTop_ is the proportion of indoors light captured by the top leaf layer [0;1]
 * - _absorptivityMiddel_ is the proportion of indoors light captured by the middle leaf layer [0;1]
 * - _absorptivityBottom_ is the proportion of indoors light captured by the bottom leaf layer [0;1]
 *
 * Output
 * ------
 * - _kDirect_ is the extinction coefficient for direct light [0;1]
 * - _kDirectDirect_ is the extinction coefficient for the direct component of direct light [0;1]
 * - _diffuseReflectivity_ is the reflectivity of diffuse light [0;1]
 * - _directReflectivity_ is the reflectivity of direct light [0;1]
 * - _reflectivity_ is the reflectivity of diffuse and direct light combined [0;1]
 */

CropRadiation::CropRadiation(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(double, kDiffuse, 0.8);
    Input(double, scattering, 0.2);
    InputRef(double, sinB, "calendar[sinB]");
    InputRef(double, lightDiffuse, "indoors/light[diffuse]");
    InputRef(double, lightDirect, "indoors/light[direct]");
    InputRef(double, absorptivityTop, "layers/top/photosynthesis[absorptivity]");
    InputRef(double, absorptivityMiddle, "layers/middle/photosynthesis[absorptivity]");
    InputRef(double, absorptivityBottom, "layers/bottom/photosynthesis[absorptivity]");

    Output(double, kDirect);
    Output(double, kDirectDirect);
    Output(double, diffuseReflectivity);
    Output(double, directReflectivity);
    Output(double, reflectivity);
}

void CropRadiation::reset() {
    kDirect = kDirectDirect = kDiffuse;
    diffuseReflectivity = directReflectivity = reflectivity = 0;
}

void CropRadiation::update() {
    double sqv = sqrt(1-scattering);

    // Reflectivity of horizontal leaf angle distribution
    diffuseReflectivity = (1-sqv)/(1+sqv);

    // Reflectivity of spherical leaf angle distribution
    directReflectivity = 2*diffuseReflectivity/(1+1.6*sinB);

    // Extinction coefficient for direct component of direct light (KdirBL)
    kDirectDirect = (sinB==0.) ? 0. : 0.5/sinB*kDiffuse/(0.8*sqv);

    // Extinction coefficient for total direct light
    kDirect = kDirectDirect*sqv;

    // Total light reflectivity weighted by diffuse vs. direct light
    double diffuse = div0(lightDiffuse, lightDiffuse+lightDirect);
    reflectivity = diffuse*diffuseReflectivity + (1-diffuse)*directReflectivity;
}

} //namespace

