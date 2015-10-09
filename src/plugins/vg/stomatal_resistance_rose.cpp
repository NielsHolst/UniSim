/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "publish.h"
#include "stomatal_resistance_rose.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(StomatalResistanceRose)

/*! \class StomatalResistanceRose
 * \brief Stomatal resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB> for pot roses
 *
 * Inputs
 * ------
 * - _co2_ is the ambient CO<SUB>2</SUB> concentration [ppm]
 * - _rh_ is indoors relative humidity [0;100]
 * - _Pn_ is net assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/h]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 * - _lai_ is the crop leaf area index [-]
 */

StomatalResistanceRose::StomatalResistanceRose(Identifier name, QObject *parent)
    : StomatalResistanceBase(name, parent)
{
    InputRef(double, co2, "indoors/co2[value]");
    InputRef(double, rh, "indoors/humidity[rh]");
    InputRef(double, Pn, "../photosynthesis[Pn]");
    InputRef(double, rbCO2, "../rb[rbCO2]");
    InputRef(double, lai, "crop/lai[value]");
}

double StomatalResistanceRose::resetRsH2O() {
    return calcRsH2O(0);
}

double StomatalResistanceRose::updateRsH2O() {
    return calcRsH2O(Pn);
}

double StomatalResistanceRose::calcRsH2O(double A) {
    const double b = 0.0960;
    const double m = 10.055;
    double Am = max(A/44/1e-6/3600, 0.); // micromole m-2 s-1
    Am *= lai;     // convert from m2 ground to m2 leaf area
    // Atmospheric pressure
    double Pa = P0/1e5; // bar
    // Internal resistance acc. Kim Lieth is in mol m-2 s-1 and recalc. with 0.025 (Jones, p. 56)
    double cs = max(co2 - Am*(rbCO2*0.025)*Pa, 0.);
    double giH2O = b + m*Am*rh/100/(cs/Pa);
    return 1./giH2O/0.025;
}


} //namespace

