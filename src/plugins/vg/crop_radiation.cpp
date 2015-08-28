/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_radiation.h"
#include "publish.h"

using std::min;
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
    Input(double, irReflectivity, 0.03);
    InputRef(double, sinB, "calendar[sinB]");
    InputRef(double, lai, "crop/lai[lai]");

    Output(double, kDirect);
    Output(double, kDirectDirect);
    Output(double, diffuseReflectivity);
    Output(double, directReflectivity);
    Output(double, diffuseAbsorptivity);
    Output(double, directAbsorptivity);
}

void CropRadiation::reset() {
    kDirect = kDiffuse;
    diffuseReflectivity = directReflectivity = 1;
    diffuseAbsorptivity = directAbsorptivity = 0;
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

    // Whole-canopy absorptivity and transmissivity
    diffuseAbsorptivity = (1-diffuseReflectivity)*(1-exp(-kDiffuse*lai));
    directAbsorptivity = (1-directReflectivity)*(1-exp(-kDirect*lai));
    diffuseTransmissivity = 1 - diffuseReflectivity - diffuseAbsorptivity;
    directTransmissivity = 1 - directReflectivity - directAbsorptivity;

    // Infra-red
    irAbsorptivity = (1-irReflectivity)*(1-exp(-kDiffuse*lai));
    irTransmissivity = 1 - irReflectivity - irAbsorptivity;

}

} //namespace

