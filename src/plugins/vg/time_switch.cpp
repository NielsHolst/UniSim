/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "time_switch.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(TimeSwitch)

/*! \class TimeSwitch
 * \brief Switches on/off according to date and time of day
 *
 * Inputs
 * ------
 * - _onDay_ is the date from when the switch can be turned on [1..365]
 * - _offDay_ is the date after which the switch is turned off [1..365]
 * - _onTime_ is the time of the day when the switch is turned on if _day_ is in the interval [_onday_,offday] [hh:mm]
 * - _offTime_ is the time after which the switch is turned off [hh:mm]
 * - _day_ is the current date [1..365]
 * - _time_ is the current time [hh:mm]
 *
 * Outputs
 * ------
 * - _timeOn_ flags whether switch is on or off [true,false]

 * Default dependencies
 * ------------
 * - a _calendar_ model with these ports:
 *   + _dayOfYear_ [1..365]
 *   + _time_ [hh:mm]
 */

TimeSwitch::TimeSwitch(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(int, onDay, 0);
    Input(int, offDay, 365);
    Input(QTime, onTime, QTime(0,0));
    Input(QTime, offTime, QTime(24,0));
    InputRef(int, day, "calendar[dayOfyear]");
    InputRef(QTime, time, "calendar[timeOfDay]");
    Output(bool, timeOn);
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

