/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
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
    InputRef(double, lai, "crop/lai[lai]");
    InputRef(double, xGauss, "..[xGauss]");
    InputRef(double, wGauss, "..[wGauss]");
    InputRef(double, LUE, "./lightResponse[LUE]");
//    InputRef(double, Pnmax, "./lightResponse[Pnmax]");
    InputRef(double, Pgmax, "./lightResponse[Pgmax]");
    InputRef(double, Rd, "./lightResponse[Rd]");
    Input(double, parProportion, 0.47);

    Output(double, parAbsorbed);
    Output(double, Pn);
    Output(double, Pg);
}

void LayerPhotosynthesis::reset() {
    Pn = Pg = 0;
}

void LayerPhotosynthesis::update() {
    laiX = lai*xGauss,
    parDiffuse = parProportion*lightDiffuse,
    parDirect = parProportion*lightDirect;

    // Compute light absorned and gross assimilation
    double absorbedShaded = absorbedByShadedLeaves(),           // [J / m2 leaf / s]
           PgShade = grossAssimilation(absorbedShaded);         // [mg CO2 / m2 leaf / s]

    auto sunlit = absorbedBySunlitLeaves(absorbedShaded);
    double absorbedSunlit = sunlit.first,                       // [J / m2 leaf / s]
           PgSunlit = sunlit.second,                            // [mg CO2 / m2 leaf / s]

           absorbedTotal = absorbedShaded + absorbedSunlit,     // [J / m2 leaf / s]
           PgTotal = grossAssimilationTotal(PgShade, PgSunlit); // [mg CO2 / m2 leaf / s]

    // Apply integration weight and convert

    // [J / m2 ground / s] = [J / m2 leaf / s * m2 leaf / m2 ground]
    parAbsorbed = absorbedTotal*wGauss*lai;

    // [mg CO2 / m2 ground / s] = [mg CO2 / m2 leaf / s * m2 leaf / m2 ground]
    Pg = PgTotal*wGauss*lai;
    Pn = Pg - Rd*wGauss*lai;

    // Convert to [g CO2 m-2 h-1]
    Pg *= 3.6;
    Pn *= 3.6;
}

double LayerPhotosynthesis::absorbedByShadedLeaves() const {
    double absorbedDiffuse = (1-diffuseReflectivity)*parDiffuse*kDiffuse*exp(-kDiffuse*laiX),
           absorbedTotal = (1-directReflectivity)*parDirect*kDirect*exp(-kDirect*laiX),
           absorbedDirect = (1-scattering)*parDirect*kDirectDirect*exp(-kDirectDirect*laiX);
    return absorbedDiffuse + absorbedTotal - absorbedDirect; // [J/m2/leaf/s]
}

QPair<double, double> LayerPhotosynthesis::absorbedBySunlitLeaves(double absorbedShaded) const {
    const double xGauss[3] = {0.1127, 0.5, 0.8873},
                 wGauss[3] = {0.2778, 0.4444, 0.2778};
    if (Pgmax==0 || sinB==0) return qMakePair(0.,0.);

    // Direct flux absorbed by leaves perpendicular on direct beam (VISpp)[J*m-2 leaf s-1]
    double absorbedPerpendicular = (1-scattering)*parDirect/(sinB);
    // Integration over all leaf angles
    double assimilationSum{0}, absorbedSum{0};
    for(int i=0; i<3; ++i) {
        // Total absorbed flux of sunlit leaves [J*m-2 leaf s-1]
        double absorbedSunlit = absorbedShaded + absorbedPerpendicular*xGauss[i];
        absorbedSum += wGauss[i]*absorbedSunlit;
        // Gross assimilation of sunlit leaves [mg CO2in*m-2 leaf s-1]
        assimilationSum += wGauss[i]*grossAssimilation(absorbedSunlit);
    }
    return qMakePair(absorbedSum, assimilationSum);
}

double LayerPhotosynthesis::grossAssimilationTotal(double Pgshade, double PgSunlit) const {
    // Proportion sunlit leaf area
    double propSunlit = exp(-kDirectDirect*laiX);
    return propSunlit*PgSunlit + (1-propSunlit)*Pgshade;
}

double LayerPhotosynthesis::grossAssimilation(double absorbed) const {
    return (Pgmax==0) ? 0 : Pgmax*(1-exp(-absorbed*LUE/Pgmax));
}

} //namespace

