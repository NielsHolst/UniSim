/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "leaf_photosynthesis.h"
#include "publish.h"

using namespace UniSim;
using namespace std;

namespace vg {
	
PUBLISH(LeafPhotosynthesis)

/*! \class LeafPhotosynthesis
 * \brief Calculates assimilation
 *
 * Inputs
 * ------
 * - _parDirect_ is the intensity of direct PAR indoors [W/m<SUP>2</SUP>]
 * - _parDiffuse_ is the intensity of diffuse PAR indoors [W/m<SUP>2</SUP>]
 * - _kDiffuse_ is the extinction coefficient for diffuse light [0;1]
 * - _kDirect_ is the extinction coefficient for direct light [0;1]
 * - _kDirectDirect_ is the extinction coefficient for the direct component of direct light [0;1]
 * - _scattering_ is the scattering coefficent for direct light [0;1]
 * - _diffuseReflectivity_ is the reflectivity of diffuse light [0;1]
 * - _directReflectivity_ is the reflectivity of direct light [0;1]
 * - _sinB_ is the sine of the sun elevation [0;1]
 * - _lai_ is the crop leaf area index [-]
 * - _LUE_ is the light use efficiency [mg CO<SUB>2</SUB>/J]
 * - _Pgmax_ is the gross assimilation rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _Rd_ is the dark respiration rate [mg CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/s]
 * - _xGauss_ is the LAI coefficient coming from above [0;1]
 * - _wGauss_ is weighing coefficient coming from above[0;1]
 *
 * Outputs
 * ------
 * - _absorptivity_ is the proportion of indoors light captured by this leaf layer [0;1]
 * - _parAbsorbed_ is the flux of PAR absorbed by this leaf layer [W/m<SUP>2</SUP> ground]
 * - _Pn_ is the net assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> ground/h]
 * - _Pg_ is the gross assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> ground/h]
 */

/* Canopy height (H) is arbitrarily set to 2 m.
 * This will only have an effect, in case Gaussian integration is carried out over canopy height,
 * rather than over LAI (current implementation).
*/
const double H{2};

// Leaf area density (m2/m3) at the Gausian point; constant for a uniform vertival LAI distribution
double LeafPhotosynthesis::lad() const {
//    double h = xGauss*H;
//    return lai*6*h*(H-h)/p3(H);
    return lai/H;
}

// LAI above the Gaussian point (xGauss)
double LeafPhotosynthesis::laic() const {
//    double h = xGauss*H;
//    return lai*( 1 - 1/p3(H)*p2(h)*(3*H - 2*h) );
    return lai*xGauss;
}

LeafPhotosynthesis::LeafPhotosynthesis(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, parDiffuse, "indoors/light[parDiffuse]");
    InputRef(double, parDirect, "indoors/light[parDirect]");
    InputRef(double, kDiffuse, "crop/radiation[kDiffuse]");
    InputRef(double, kDirect, "crop/radiation[kDirect]");
    InputRef(double, kDirectDirect, "crop/radiation[kDirectDirect]");
    InputRef(double, scattering, "crop/radiation[scattering]");
    InputRef(double, diffuseReflectivity, "crop/radiation[diffuseReflectivity]");
    InputRef(double, directReflectivity, "crop/radiation[directReflectivity]");
    InputRef(double, lai, "crop/lai[value]");
    InputRef(double, sinB, "calendar[sinB]");

    InputRef(double, LUE, "./lightResponse[LUE]");
    InputRef(double, Pgmax, "./lightResponse[Pgmax]");
    InputRef(double, Rd, "./lightResponse[Rd]");
    InputRef(double, xGauss, "..[xGaussUpperside]");
    InputRef(double, wGauss, "..[wGaussUpperside]");

    Output(double, absorptivity);
    Output(double, parAbsorbed);
    Output(double, Pn);
    Output(double, Pg);
}

void LeafPhotosynthesis::reset() {
    absorptivity = parAbsorbed = Pn = Pg = 0;
}

void LeafPhotosynthesis::update() {
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

double LeafPhotosynthesis::absorbedByShadedLeaves() const {
    double laic_ = laic();
    double absorbedDiffuse = (1-diffuseReflectivity)*parDiffuse*kDiffuse*exp(-kDiffuse*laic_),
           absorbedTotal = (1-directReflectivity)*parDirect*kDirect*exp(-kDirect*laic_),
           absorbedDirect = (1-scattering)*parDirect*kDirectDirect*exp(-kDirectDirect*laic_);
    return absorbedDiffuse + absorbedTotal - absorbedDirect; // [J/m2/leaf/s]
}

QPair<double, double> LeafPhotosynthesis::absorbedBySunlitLeaves(double absorbedShaded) const {
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


double LeafPhotosynthesis::grossAssimilation(double absorbed) const {
    return (Pgmax==0) ? 0 : Pgmax*(1-exp(-absorbed*LUE/Pgmax));
}

} //namespace

