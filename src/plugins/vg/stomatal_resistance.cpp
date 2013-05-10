/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "stomatal_resistance.h"

using namespace UniSim;

namespace vg {
	
StomatalResistance::StomatalResistance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(rh), "indoors/humidity[rh]");
    addParameter<double>(Name(co2), 500., "Air CO2 [ppm]");
    addParameter<double>(Name(Pgc), 0., "Photosynthetic rate (g CO2/m2/h)");
    addParameterRef<double>(Name(rbH2O), "boundaryLayerResistance[rbH2O]");
    addVariable<double>(Name(riH2O), "Stomatal resistance against water vapour (s/m)");
}

void StomatalResistance::reset() {
    update();
}

void StomatalResistance::update() {
    const double Pa = 1.013;        // Atmospheric pressure kPa
    const double b = 0.0960;
    const double ms = 10.055;

    double Pgcmmol= 1e6/44*Pgc/3600;
    // Internal resistance acc. Kim Lieth is in mol m-2 s-1 and recalc. with 0.025 (Jones, p. 56)
    double cs = co2 - Pgcmmol*(1.37*rbH2O*0.025)*Pa;
    double giH2O =b + ms*Pgcmmol*rh/100/(cs/Pa);
    riH2O = 1/giH2O/0.025;
}


} //namespace

