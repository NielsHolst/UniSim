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
    new Parameter<int>("day", &day, 1, this, "Julian day");
    new Parameter<int>("hour", &hour, 12, this, "Hour of the day");
    new Parameter<int>("minute", &minute, 0, this, "Minute of the hour");
    new Parameter<double>("globRad", &globRad, 280., this, "Global outdoors radiation (W/m2)");
    new Parameter<int>("spDayOn", &spDayOn, 300, this, "Julian day when to switch on (1..365)");
    new Parameter<int>("spDayOff", &spDayOff, 60, this, "Julian day when to switch off (1..365)");
    new Parameter<double>("spHourOn", &spHourOn, 8, this, "Hour when to switch on (0..24)");
    new Parameter<double>("spHourOff", &spHourOff, 24, this, "Hour when to switch off (0..24)");
    new Parameter<double>("spRadiationOn", &spRadiationOn, 250, this, "Radiation intensity at which to switch on (W/m2)");
    new Parameter<double>("spRadiationOff", &spRadiationOff, 300, this, "Radiation intensity at which to switch off (W/m2)");

    new Variable<bool>("on", &on, this, "Switch lamps on?");
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

