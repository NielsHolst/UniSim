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
    new Parameter<double>("rh", &rh, 80., this, "Relative humidity of the air [0;100]");
    new Parameter<double>("co2", &co2, 500., this, "Air CO2 [ppm]");
    new Parameter<double>("Pgc", &Pgc, 0., this, "Photosynthetic rate (g CO2/m2/h)");
    new Parameter<double>("rbH2O", &rbH2O, 0., this, "Boundary layer resistance against water vapour");
    new Variable<double>("riH2O", &riH2O, this, "Stomatal resistance against water vapour (s/m)");
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

