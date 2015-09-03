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
 * \brief
 *
 * Inputs
 * ------

 * Output
 * ------
 *
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
    Output(double, transmissivity);
}

void CropRadiation::reset() {
    kDirect = kDiffuse;
    diffuseReflectivity = directReflectivity = 0;
    transmissivity = 1;
}

void CropRadiation::update() {
    double sqv = sqrt(1-scattering);

    // Reflectivity of horizontal leaf angle distribution
    diffuseReflectivity = (1-sqv)/(1+sqv);

    // Reflectivity of spherical leaf angle distribution
    directReflectivity = 2*diffuseReflectivity/(1+1.6*sinB);

    // Extinction coefficient for direct component of direct PAR flux (KdirBL)
    kDirectDirect = (sinB==0.) ? 0. : 0.5/sinB*kDiffuse/(0.8*sqv);

    // Extinction coefficient for total direct PAR flux
    kDirect = kDirectDirect*sqv;

    // Total light reflectivity weighted by diffuse vs. direct light
    double diffuse = div0(lightDiffuse, lightDiffuse+lightDirect);
    reflectivity = diffuse*diffuseReflectivity + (1-diffuse)*directReflectivity;

    // At low sun elevation the radiation budget may not add up to 1, then transmissivity is set to zero
    transmissivity = max(1 - reflectivity - absorptivityTop - absorptivityMiddle - absorptivityBottom, 0.);
}

} //namespace

