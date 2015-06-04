/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "publish.h"
#include "stomatal_resistance.h"

using namespace UniSim;

namespace vg {

PUBLISH(StomatalResistance)

/*! \class StomatalResistance
 * \brief Stomatal resistance against H<SUB>2</SUB>O and CO<SUB>2</SUB>
 *
 * Inputs
 * ------
 * - _co2_ is the ambient CO<SUB>2</SUB> concentration [ppm]
 * - _rh_ is indoors relative humidity [0;100]
 * - _Pg_ is gross assimilation rate [g CO<SUB>2</SUB>/m<SUP>2</SUP> leaf/h]
 * - _rbCO2_ is the boundary layer resistance against CO<SUB>2</SUB> [s/m]
 *
 * Outputa
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

StomatalResistance::StomatalResistance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, co2, "indoors/co2[ppm]");
    InputRef(double, rh, "indoors/humidity[rh]");
    InputRef(double, Pg, "../photosynthesis[Pg]");
    InputRef(double, rbCO2, "../rb[rbCO2]");
    Output(double, rsH2O);
    Output(double, rsCO2);
}

void StomatalResistance::reset() {
    rsH2O = 2000.; // At night,  GCC, p. 145
    rsCO2 = rsH2O*1.6;
}

void StomatalResistance::update() {
    const double b = 0.0960;
    const double ms = 10.055;
    double PgMol = Pg/44/1e-6/3600; // micromole m-2 s-1
    // Atmospheric pressure MPa
    double Pa = P0/1e6;
    // Internal resistance acc. Kim Lieth is in mol m-2 s-1 and recalc. with 0.025 (Jones, p. 56)
    double cs = co2 - PgMol*(rbCO2*0.025)*Pa;
    double giH2O = b + ms*PgMol*rh/100/(cs/Pa);
    rsH2O = 1./giH2O/0.025;
    rsCO2 = rsH2O*1.6;
}

} //namespace

