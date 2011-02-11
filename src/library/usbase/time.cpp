/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "time.h"

namespace UniSim {

QMap<Time::Unit, char> Time::_unitToChar;
QMap<char, Time::Unit> Time::_charToUnit;


Time::Time(int time, Unit unit)
    : _time(time), _unit(unit)
{
}

char Time::unitToChar(Unit unit) {
    if (_unitToChar.isEmpty()) {
        _unitToChar[Seconds] = 's';
        _unitToChar[Minutes] = 'm';
        _unitToChar[Hours] = 'h';
        _unitToChar[Days] = 'd';
        _unitToChar[Years] = 'y';
    }
    return _unitToChar[unit];
}

Time::Unit Time::charToUnit(char ch) {
    if (_charToUnit.isEmpty()) {
        _charToUnit['s'] = Seconds;
        _charToUnit['m'] = Minutes;
        _charToUnit['h'] = Hours;
        _charToUnit['d'] = Days;
        _charToUnit['y'] = Years;
    }
    return _charToUnit[ch];
}


int Time::time() const {
    return _time;
}

Time::Unit Time::unit() const {
    return _unit;
}

QDateTime operator+(const QDateTime &dateTime, const Time &time) {
    int dt = time.time();
    switch (time.unit()) {
    case Time::Seconds:
        return dateTime.addSecs(dt);
    case Time::Minutes:
        return dateTime.addSecs(60*dt);
    case Time::Hours:
        return dateTime.addSecs(60*60*dt);
    case Time::Days:
        return dateTime.addDays(dt);
    case Time::Years:
        QDate date = dateTime.date();
        QTime savedTime = dateTime.time();
        date = QDate(date.year() + dt, date.month(), date.day());
        return QDateTime(date, savedTime, dateTime.timeSpec());
    }
    Q_ASSERT(false);
    return QDateTime();
}

QDateTime operator+(const Time &time, const QDateTime &dateTime) {
    return dateTime + time;
}

}

