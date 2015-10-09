/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "publish.h"
#include "leaf_temperature.h"

using namespace UniSim;

namespace vg {

PUBLISH(LeafTemperature)

/*! \class LeafTemperature
 * \brief Temperature of canopy layer
 *
 * Inputs
 * ------
 * - _indoorsTemperature_ is the ambient indoors temperature [<SUP>o</SUP>C]
 * - _indoorsRh_ is the ambient indoors relative humidity [0;100]
 * - _rsH2O_ is the stomatal resistance against water vapour [s/m]
 * - _rbH2O_ is the boundary layer resistance against water vapour [s/m]
 * - _radiationAbsorbed_ is the absorbed radiation [W/m<SUP>2</SUP>]
 *
 * Output
 * ------
 * - _value_ is the leaf temperature [<SUP>o</SUP>C]
 */

LeafTemperature::LeafTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, indoorsRh, "indoors/humidity[rh]");
    InputRef(double, rsH2O, "../rs[rsH2O]");
    InputRef(double, rbH2O, "../rb[rbH2O]");
    InputRef(double, radiationAbsorbed, "../radiationAbsorbed[value]");
    Output(double, value);
}

void LeafTemperature::reset() {
    value = indoorsTemperature;
}

void LeafTemperature::update() {
    double s = svpSlope(indoorsTemperature),
           psatu = svp(indoorsTemperature),
           pgh = vpFromRh(indoorsTemperature, indoorsRh),
           Tgh3 = p3(indoorsTemperature+T0);

    value = (1/RhoAir/CpAir*(rsH2O+rbH2O)*radiationAbsorbed - 1/Psychr*(psatu-pgh))
            /
            (1+(s/Psychr+ rsH2O/rbH2O+ 1/(RhoAir*CpAir/4/Sigma*Tgh3)*(rsH2O+rbH2O)))
            + indoorsTemperature;
    if (std::isnan(value)) {
        throw Exception("LeafTemperature is not a number");
    }

    /* Thermal storage was neglible, max 1-2 W/m2
    double dt = 300,
           sla = 30,
           C = 300;
    dT = value-prevValue;
    thermalStorage = dT*lai/sla*C/dt;
    */
}


} //namespace

