/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "day_or_night_by_light.h"

using namespace UniSim;

namespace vg {
	
DayOrNightByLight::DayOrNightByLight(Identifier name, QObject *parent)
    : DayOrNightBase(name, parent)
{
    addParameter<double>(Name(lightThreshold), 10., "Threshold for daylight (W/m2)");
    addParameterRef<double>(Name(outdoorsRadiation), "outdoors[radiation]");
}

void DayOrNightByLight::update() {
    isDay = outdoorsRadiation > lightThreshold;
}

} //namespace

