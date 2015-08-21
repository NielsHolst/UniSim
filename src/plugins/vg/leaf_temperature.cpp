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
 * \brief Temperatures of leaves in a canopy layer
 *
 * Inputs
 * ------
 * - _Tgh_ is the ambient indoors temperature [<SUP>o</SUP>C]
 * - _RHgh_ is the ambient indoors relative humidity [0;100]
 * - _rsH2O_ is the stomatal resistance against water vapour [s/m]
 * - _rbH2O_ is the boundary layer resistance against water vapour [s/m]
 * - _rna_ is the absorbed radiation [W/m<SUP>2</SUP>]
 *
 * Output
 * ------
 * - _value_ is the leaf temperature [<SUP>o</SUP>C]
 *
 * Default dependencies
 * ------------
 * - an _indoors/temperature_ model with a _value_ port [<SUP>o</SUP>C]
 * - an _indoors/humidity_ model with an _rh_ port [0;100]
 * - an _rs_ sibling model with an _rsH2O_ port [s/m]
 * - an _rb_ sibling model with an _rbH2O_ port [s/m]
 * - an _rna_ sibling model with an _rna_ port [W/m<SUP>2</SUP>]
 */

LeafTemperature::LeafTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, Tgh, "indoors/temperature[value]");
    InputRef(double, RHgh, "indoors/humidity[rh]");
    InputRef(double, rsH2O, "../rs[rsH2O]");
    InputRef(double, rbH2O, "../rb[rbH2O]");
    InputRef(double, rna, "../rna[rna]");
    Output(double, value);
}

void LeafTemperature::reset() {
    value = Tgh;
}

void LeafTemperature::update() {
    double s = svpSlope(Tgh);
    double psatu = svp(Tgh);
    double pgh = vpFromRh(Tgh, RHgh);
    double Tgh3 = pow((Tgh+T0), 3);
    value = (1/RhoAir/CpAir*(rsH2O+rbH2O)*rna - 1/Psychr*(psatu-pgh))
            /
            (1+(s/Psychr+ rsH2O/rbH2O+ 1/(RhoAir*CpAir/4/Sigma*Tgh3)*(rsH2O+rbH2O)))
            + Tgh;
}


} //namespace
