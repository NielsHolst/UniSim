/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "publish.h"
#include "stomatal_resistance_tomato.h"

using std::min;
using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(StomatalResistanceTomato)

/*! \class StomatalResistanceTomato
 * \brief Stomatal resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _co2_ is the ambient CO<SUB>2</SUB> concentration [ppm]
 * - _rh_ is indoors relative humidity [0;100]
 * - _Pg_ is gross assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/h]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 *
 * Outputs
 * ------
 * - _rbH2O_ is the stomatal resistance against water vapour [s/m]
 * - _rbCO2_ is the stomatal resistance against CO<SUB>2</SUB> [s/m]
 *
 * Default dependencies
 * ------------
 * - an _indoors/co2_ model with a _ppm_ port [ppm]
 * - an _indoors/humidity_ model with an _rh_ port [0;100]");
 * - a _photosynthesis_ sibling model with a _Pg_ port [g CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/h]
 * - an _rb_ sibling model with an _rbCO2_ port [s/m]
 */

StomatalResistanceTomato::StomatalResistanceTomato(Identifier name, QObject *parent)
    : StomatalResistanceBase(name, parent)
{
    Input(double, riH2Omin, 82.);
    InputRef(double, lai, "crop/lai[value]");
    InputRef(double, temperature, "../temperature[value]");
    InputRef(double, rh, "indoors/humidity[rh]");
    InputRef(double, radiationAbsorbed, "../radiationAbsorbed[value]");
    InputRef(double, co2, "indoors/co2[value]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
}

double StomatalResistanceTomato::resetRsH2O() {
    return riH2Omin;
}

double StomatalResistanceTomato::updateRsH2O() {
    if (lai==0.) return riH2Omin;
    // Stanghellini (page 37, 87)
    // stomatal (internal) resistance to H2O [s/m] as a function of irrad, Temp, CO2, RH
    bool atNight = (radiationAbsorbed < 3);
    double vpd = vpdFromRh(indoorsTemperature, rh),
           radAbs = max(radiationAbsorbed/(2*lai), 0.),
           fRadiation = (radAbs + 4.3)/(radAbs + 0.54),
           fTemperature = 1 + 2.2593e-2*sqr(temperature + T0 - 297.66),
           fCO2 = atNight ? 1 : min(1 + 6.08e-7*sqr(co2-200), 1.49),
           fHumidity = 4/pow((1 + 255*exp(-0.5427*vpd*0.01)), 0.25);
//    double test = riH2Omin*fRadiation*fTemperature*fCO2*fHumidity;
    return riH2Omin*fRadiation*fTemperature*fCO2*fHumidity;
}


} //namespace

