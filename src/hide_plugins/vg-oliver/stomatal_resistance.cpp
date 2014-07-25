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

StomatalResistance::StomatalResistance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, rh, "indoors/humidity[rh]");
    Input(double, co2, 700.);
    InputRef(double, Pgc, "../photosynthesis[Pg]");
    InputRef(double, rbCO2, "../rb[rbCO2]");
    Output(double, rsH2O);  // (s/m)
    Output(double, rsCO2);  // (s/m)
}

void StomatalResistance::reset() {
    updateValue(0.);
}

void StomatalResistance::update() {
    updateValue(Pgc/44/1e-6);
}

void StomatalResistance::updateValue(double PgcMol) { //PgcMol in micromole m-2 s-1
    const double b = 0.0960;
    const double ms = 10.055;
    // Atmospheric pressure MPa
    double Pa = P0/1e6;
    // Internal resistance acc. Kim Lieth is in mol m-2 s-1 and recalc. with 0.025 (Jones, p. 56)
    double cs = co2 - PgcMol*(rbCO2*0.025)*Pa;
    double giH2O = b + ms*PgcMol*rh/100/(cs/Pa);
    rsH2O = 1./giH2O/0.025;
    rsCO2 = rsH2O*1.6;
}

} //namespace

