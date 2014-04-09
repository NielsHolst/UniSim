/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "time_switch.h"

using namespace UniSim;

namespace vg {
	
TimeSwitch::TimeSwitch(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<int>(Name(onDay), 0, "Julian day when to switch on (1..365)");
    addParameter<int>(Name(offDay), 365, "Julian day when to switch off (1..365)");
    addParameter<QTime>(Name(onTime), QTime(0,0), "Time when to switch on");
    addParameter<QTime>(Name(offTime), QTime(24,0), "Time when to switch off");

    addParameterRef<int>(Name(day), "calendar[dayOfyear] ");
    addParameterRef<QTime>(Name(time), "calendar[timeOfDay]");

    addVariable<bool>(Name(timeOn), "Switch on according to current time?");
}

void TimeSwitch::reset() {
    timeOn = false;
}

void TimeSwitch::update() {
    bool nowOnDay = (onDay < offDay) ?
                 (day >= onDay) && (day <= offDay) :
                 (day >= onDay) || (day <= offDay);
    bool nowOnTime = (onTime < offTime) ?
                  (time >= onTime) && (time <= offTime) :
                  (time >= onTime) || (time <= offTime);
    timeOn = nowOnDay && nowOnTime;
}

} //namespace

