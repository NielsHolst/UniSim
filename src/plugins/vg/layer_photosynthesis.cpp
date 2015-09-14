/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "layer_photosynthesis.h"
#include "publish.h"

using namespace UniSim;
using namespace std;

namespace vg {
	
PUBLISH(LayerPhotosynthesis)

/*! \class LayerPhotosynthesis
 * \brief Assimilation of a canopy layer
 *
 * Inputs
 * ------
 * - _sinb_ is the sine of sun elevation [-1;1]
 * - _lightDif_ is the indoors diffuse light [W//m<SUP>2</SUP>]
 * - _lightDir_ is the indoors direct light [W//m<SUP>2</SUP>]
 * - _lai_ is leaf area index [m<SUP>2</SUP>/m<SUP>2</SUP>]
 * - _xGauss_ is the LAI coefficient [0;1]
 * - _wGauss_ is weighing coefficient [0;1]
 * - _LUE_ is the light use efficiency [mg CO<SUB>2</SUB>/J]
 * - _Pnmax_ is the maximum net assimilation rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _Pgmax_ is the maximum gross assimilation rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _Rd_ is the dark respiration rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _Kdif_ is extinction coefficient for difuse light [0;1]
 * - _SCV_ is light scattering coefficient [0;1]
 *
 * Outputs
 * ------
 * - _Pn_ is the net assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> ground/h]
 * - _Pg_ is the gross assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> ground/h]
 */

const double H{2};

double LayerPhotosynthesis::lad() const {
//    double h = xGauss*H;
//    return lai*6*h*(H-h)/p3(H);
    return lai/H;
}

double LayerPhotosynthesis::laic() const {
//    double h = xGauss*H;
//    return lai*( 1 - 1/p3(H)*p2(h)*(3*H - 2*h) );
    return lai*xGauss;
}

LayerPhotosynthesis::LayerPhotosynthesis(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, kDiffuse, "crop/radiation[kDiffuse]");
    InputRef(double, kDirect, "crop/radiation[kDirect]");
    InputRef(double, kDirectDirect, "crop/radiation[kDirectDirect]");
    InputRef(double, scattering, "crop/radiation[scattering]");
    InputRef(double, diffuseReflectivity, "crop/radiation[diffuseReflectivity]");
    InputRef(double, directReflectivity, "crop/radiation[directReflectivity]");

    InputRef(double, sinB, "calendar[sinB]");
    InputRef(double, lightDiffuse, "indoors/light[diffuse]");
    InputRef(double, lightDirect, "indoors/light[direct]");
    Input(double, parProportion, 0.47);
    InputRef(double, growthLightPar, "growthLights[parEmission]");
    InputRef(double, lai, "crop/lai[value]");
    InputRef(double, xGauss, "..[xGaussUpperside]");
    InputRef(double, wGauss, "..[wGaussUpperside]");
    InputRef(double, LUE, "./lightResponse[LUE]");
    InputRef(double, Pgmax, "./lightResponse[Pgmax]");
    InputRef(double, Rd, "./lightResponse[Rd]");

    InputRef(double, lat, "calendar[latitude]");
    InputRef(int, day, "calendar[dayOfYear]");
    InputRef(int, hour, "calendar[hour]");
    InputRef(int, minute, "calendar[minute]");

    Output(double, absorptivity);
    Output(double, parAbsorbed);
    Output(double, Pn);
    Output(double, Pg);
}

void LayerPhotosynthesis::reset() {
    absorptivity = parAbsorbed = Pn = Pg = 0;
}

void LayerPhotosynthesis::update() {
    parDiffuse = parProportion*lightDiffuse;
    parDirect = parProportion*lightDirect + growthLightPar;

    // Compute light absorned and gross assimilation
    double absorbedShaded = absorbedByShadedLeaves(),           // [J / m2 leaf / s]
           PgShade = grossAssimilation(absorbedShaded);         // [mg CO2 / m2 leaf / s]

    auto sunlit = absorbedBySunlitLeaves(absorbedShaded);
    double absorbedSunlit = sunlit.first,                       // [J / m2 leaf / s]
           PgSunlit = sunlit.second,                            // [mg CO2 / m2 leaf / s]
           propSunlit = exp(-kDirectDirect*laic()),
           absorbedTotal = (propSunlit*absorbedSunlit + (1-propSunlit)*absorbedShaded),
           PgTotal = propSunlit*PgSunlit + (1-propSunlit)*PgShade,
           integrationWeight = lad()*wGauss*H;

    // Apply integration weight and convert

    // [J / m2 ground / s] = [J / m2 leaf / s * m2 leaf / m2 ground]
//    parAbsorbed = absorbedTotal*wGauss*lai;
    parAbsorbed = absorbedTotal*integrationWeight;
    absorptivity = div0(parAbsorbed, parDiffuse + parDirect);

    // [mg CO2 / m2 ground / s] = [mg CO2 / m2 leaf / s * m2 leaf / m2 ground]
    Pg = PgTotal*integrationWeight;
    Pn = Pg - Rd*integrationWeight;

    // Convert to [g CO2 m-2 h-1]
    Pg *= 3.6;
    Pn *= 3.6;
}

double LayerPhotosynthesis::absorbedByShadedLeaves() const {
    double laic_ = laic();
    double absorbedDiffuse = (1-diffuseReflectivity)*parDiffuse*kDiffuse*exp(-kDiffuse*laic_),
           absorbedTotal = (1-directReflectivity)*parDirect*kDirect*exp(-kDirect*laic_),
           absorbedDirect = (1-scattering)*parDirect*kDirectDirect*exp(-kDirectDirect*laic_);
    return absorbedDiffuse + absorbedTotal - absorbedDirect; // [J/m2/leaf/s]
}

QPair<double, double> LayerPhotosynthesis::absorbedBySunlitLeaves(double absorbedShaded) const {
    if (Pgmax==0 || sinB==0) return qMakePair(0.,0.);

    // Direct flux absorbed by leaves perpendicular on direct beam (VISpp)[J*m-2 leaf s-1]
    // Original eq. changed to guard against sinB->0 yielding absorptivity>1
    double absorptivity = min((1-scattering)/sinB, 1.);
    double absorbedPerpendicular = absorptivity*parDirect;
    // Integration over all leaf angles
    double assimilationSum{0}, absorbedSum{0};
    for(int i=0; i<3; ++i) {
        // Total absorbed flux of sunlit leaves [J*m-2 leaf s-1]
        double absorbedSunlit = absorbedShaded + absorbedPerpendicular*xGauss3[i];
        absorbedSum += wGauss3[i]*absorbedSunlit;
        // Gross assimilation of sunlit leaves [mg CO2in*m-2 leaf s-1]
        assimilationSum += wGauss3[i]*grossAssimilation(absorbedSunlit);
    }
    return qMakePair(absorbedSum, assimilationSum);
}


double LayerPhotosynthesis::grossAssimilation(double absorbed) const {
    return (Pgmax==0) ? 0 : Pgmax*(1-exp(-absorbed*LUE/Pgmax));
}

} //namespace

