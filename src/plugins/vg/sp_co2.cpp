/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "sp_co2.h"

using namespace UniSim;

namespace vg {
	
SpCo2::SpCo2(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("globRad", &globRad, 0., this, "Global radiation (W/m2)");
    new Parameter<double>("spVentilationOpening", &spVentilationOpening, 0., this, "Setpoint for ventilation opening [0;100]");
    new Variable<double>("sp", &sp, this, "Setpoint for CO2 (ppm)");
}

void SpCo2::reset() {
    update();
}

void SpCo2::update() {
    sp = (globRad > 0) ? 1000. : 0.;
    if (spVentilationOpening >= 5.)
        sp = 350.;
}


} //namespace

