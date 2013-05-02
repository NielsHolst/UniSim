/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "lamp_switch.h"

using namespace UniSim;

namespace vg {
	
LampSwitch::LampSwitch(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<int>(Name(day), "calendar[dayOfyear] ");
    addParameterRef<int>(Name(hour), "calendar[hour]");
    addParameterRef<int>(Name(minute),"calendar[minute]");
    addParameterRef<double>(Name(globRad), "outdoors/records[globRad]");

    addParameter<int>(Name(spDayOn), 300, "Julian day when to switch on (1..365)");
    addParameter<int>(Name(spDayOff), 60, "Julian day when to switch off (1..365)");
    addParameter<double>(Name(spHourOn), 8, "Hour when to switch on (0..24)");
    addParameter<double>(Name(spHourOff), 24, "Hour when to switch off (0..24)");
    addParameter<double>(Name(spRadiationOn), 250, "Radiation intensity at which to switch on (W/m2)");
    addParameter<double>(Name(spRadiationOff), 300, "Radiation intensity at which to switch off (W/m2)");

    addVariable<bool>(Name(on), "Switch lamps on?");
}

void LampSwitch::reset() {
    on = false;
}

void LampSwitch::update() {
    double time = hour + minute/60.;
    bool dayOk = (day >= spDayOn) || (day <= spDayOff);
    bool hourOk = (time >= spHourOn) && (time <= spHourOff);
    bool radiationOk = (globRad >= spRadiationOn) && (globRad <= spRadiationOff);
    on = dayOk && hourOk && radiationOk;
}


} //namespace

