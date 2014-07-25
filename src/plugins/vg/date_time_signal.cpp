/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "date_time_signal.h"

using namespace UniSim;

namespace vg {

PUBLISH(DateTimeSignal)

/*! \class DateTimeSignal
 * \brief Sets a signal according to date and time of day
 *
 * The day and time intervals are closed,
 * i.e. values equal to the beginning or the end interval of an interval are considered inside the interval.
 *
 * Inputs
 * ------
 * - _beginDay_ is the beginning of the date interval [1..365]
 * - _endDay_ is the end of the date interval [1..365]
 * - _beginTime_ is the beginning of the time interval [hh:mm]
 * - _endTime_ is the end of the time interval [hh:mm]
 * - _day_ is the actual date [1..365]
 * - _time_ is the actual time [hh:mm]
 * - _signalInside_ is the resulting _signal_ when _day_ and _time_ are inside the day and time intervals [R]
 * - _signalOutsideDate_ is the resulting _signal_ when _day_ is outside the date interval [R]
 * - _signalOutsideTime_ is the resulting _signal_ when _day_ is inside the date interval but _time_ is outside the time interval [R]
 *
 * Outputs
 * ------
 * See BaseSignal
 *
 * Default dependencies
 * ------------
 * - an _calendar_ model with two ports:
 *   + _dayOfYear_ [1..365]
 *   + _timeOfDay_ [hh:mm]
  */

DateTimeSignal::DateTimeSignal(Identifier name, QObject *parent)
    : BaseSignal(name, parent){
    Input(int, beginDay, 1);
    Input(int, endDay, 365);
    Input(QTime, beginTime, QTime(0,0));
    Input(QTime, endTime, QTime(24,0));
    InputRef(int, day, "calendar[dayOfyear]");
    InputRef(QTime, time, "calendar[timeOfDay]");
    Input(double, signalOutsideDate, 0.);
    Input(double, signalOutsideTime, 0.);
    Input(double, signalInside, 1.);
}

double DateTimeSignal::signal() {
    bool nowOnDay = (beginDay < endDay) ?
                (day >= beginDay) && (day <= endDay) :
                (day >= beginDay) || (day <= endDay);
    bool nowOnTime = (beginTime < endTime) ?
                (time >= beginTime) && (time <= endTime) :
                (time >= beginTime) || (time <= endTime);
    return nowOnDay ?
           (nowOnTime ? signalInside : signalOutsideTime)
           : signalOutsideDate;
}

} //namespace

