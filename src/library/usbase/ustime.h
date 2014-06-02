/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TIME_H
#define UNISIM_TIME_H
#include <QDateTime>
#include <QMap>
#include <QString>

class QObject;

namespace UniSim {

class Time {
public:
    enum Unit{Seconds, Minutes, Hours, Days, Years};
    Time(int time, Unit unit);
    int time() const;
    Unit unit() const;
    long long toSeconds() const;
    static char unitToChar(Unit unit);
    static Unit charToUnit(char ch, QObject *concerning = 0);
    static double conversionFactor(Unit from, Unit to);
private:
    int _time;
    Unit _unit;
    static QMap<Unit, char> _unitToChar;
    static QMap<char, Unit> _charToUnit;
    static QMap<Unit, long long> unitToSeconds;
};

QDateTime operator+(const QDateTime &dateTime, const Time &time);
QDateTime operator+(const Time &time, const QDateTime &dateTime);

} //namespace

#endif
