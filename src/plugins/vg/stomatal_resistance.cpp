/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "stomatal_resistance.h"

using namespace UniSim;

namespace vg {

StomatalResistance::StomatalResistance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(rh), "climate/humidity[rh]");
    addParameterRef<double>(Name(co2), "climate/co2/[value]");
    addParameterRef<double>(Name(PgcMol), "crop/photosynthesis[PgcMol]");
    addParameterRef<double>(Name(rbH2O), "crop/rbH2O[value]");
    addVariable<double>(Name(value), "Stomatal resistance against water vapour (s/m)");
}

void StomatalResistance::reset() {
    updateValue(0.);
}

void StomatalResistance::update() {
    updateValue(PgcMol);
}

void StomatalResistance::updateValue(double PgcMol) {
    const double b = 0.0960;
    const double ms = 10.055;
    // Atmospheric pressure MPa
    double Pa = P0/1e6;
    // Internal resistance acc. Kim Lieth is in mol m-2 s-1 and recalc. with 0.025 (Jones, p. 56)
    double cs = co2 - PgcMol*(1.37*rbH2O*0.025)*Pa;
    double giH2O = b + ms*PgcMol*rh/100/(cs/Pa);
    value = 1./giH2O/0.025;
}

} //namespace

