/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
    #include "sp_co2.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
SpCo2::SpCo2(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(globRad), "outdoors/records[globRad]");
    addParameterRef<double>(Name(alphaLeeSide), "greenhouse/ventilation/spOpening[alphaLeeSide]");
    addParameterRef<double>(Name(alphaWindSide), "greenhouse/ventilation/spOpening[alphaWindSide]");
    addVariable<double>(Name(sp), "Setpoint for CO2 (ppm)");
}

void SpCo2::reset() {
    update();
}

void SpCo2::update() {
    sp = (globRad > 0) ? 1000. : 0.;
    if (max(alphaLeeSide, alphaWindSide) >= 5.)
        sp = 350.;
}


} //namespace

