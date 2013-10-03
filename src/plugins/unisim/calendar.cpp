/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
/* Day length and solar elevation equations copied from the FORTRAN code of Kroppf &
** Laar (1993). Modelling Crop-Weed Interactions. CAB International.
*/
#include <QTextStream>
#include <cfloat>
#include <cmath>
#include <usbase/clock.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/test_num.h>
#include "calendar.h"

using namespace std;

namespace UniSim{

Calendar::Calendar(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(latitude), 52., "Latitude of simulated system");

    addParameter<QDate>(Name(initialDate), QDate(2000,1,1),
    "Initial date of simulation. "
    "You should perform a @F deepReset on your @F Calendar object after pushing a new "
    "value to @F {initialDate}.");

    addParameter<QTime>(Name(initialTimeOfDay), QTime(0,0,0),
    "Initial time of day of simulation. Default is midnight. "
    "You should perform a @F deepReset on your @F Calendar object after pushing a new "
    "value to @F {initialDate}.");

    addParameter<int>(Name(timeStep), 1,
    "Duration of one integration time step in units determined by @F {timeUnit}");

    addParameter<char>("timeUnit", &timeUnitAsChar, 'd',
    "Time unit of @F {timeStep}: s)econds, m)inutes, h)ours, d)ays or y)ears.");

    addParameter<int>(Name(timeStepOffset), -1,
    "The first values are reported to plot and table outputs after one time step. "
    "With the default value of -1 for @F timeStepOffset, the first output will occur "
    "at time zero, defined by @F initialDay and @F {initialTimeOfDay}. "
    "Often this is what is intuitively expected. With a @F timeStepOffset value of zero "
    "the first output will occur one time step after time zero.");

    addVariable<QDate>(Name(date), "Current date");
    addVariable<QTime>(Name(timeOfDay), "Current time of day");
    addVariable<QDateTime>(Name(dateTime), "Current date and time");
    addVariable<double>(Name(timeStepSecs), "Duration of @F timeStep in seconds");
    addVariable<int>(Name(totalTimeSteps),
    "Total number of time steps performed since beginning of simulation");
    addVariable<int>(Name(totalTime),
    "Total time, in units determined by @F {timeUnit}. passed since beginning of simulation");
    addVariable<double>(Name(totalDays),
    "Total days passed since beginning of simulation");
    addVariable<int>(Name(dayOfYear), "Day number in year, also known as Julian day");
    addVariable<int>(Name(day), "Current day in month (1..31)");
    addVariable<int>(Name(month), "Current month (1..12)");
    addVariable<int>(Name(year), "Current year");
    addVariable<int>(Name(hour), "Current hour of the day (0..23)");
    addVariable<int>(Name(minute), "Current minute of the hour (0..59)");
    addVariable<int>(Name(second), "Current second of the minute (0..59)");

    addVariable<double>(Name(dateAsReal), "Date as a real number measured in years");
    addVariable<double>(Name(dayLength), "Current day length (hours)");
    addVariable<double>(Name(sinb), "Sine of sun elevation, updated by the @F tick event of the @F clock object");
    addVariable<double>(Name(sinLD), "Intermediate variable in astronomic calculations, updated by the @F tick event of the @F clock object");
    addVariable<double>(Name(cosLD), "Intermediate variable in astronomic calculations, updated by the @F tick event of the @F clock object");
}

void Calendar::initialize() {
    connect(clock(), SIGNAL(tick(double)), this, SLOT(handleClockTick(double)));
}

void Calendar::reset() {
    timeUnit = Time::charToUnit(timeUnitAsChar);
    dateTime = QDateTime(initialDate, initialTimeOfDay, Qt::UTC);
    dateTime = dateTime + Time(timeStep*timeStepOffset, timeUnit);
    totalTimeSteps = 0;
    timeStepSecs = timeStep*Time::conversionFactor(timeUnit, Time::Seconds);
    updateDerived();
}

void Calendar::update() {
    ++totalTimeSteps;
    dateTime = dateTime + Time(timeStep, timeUnit);
    updateDerived();
}

void Calendar::updateDerived() {
    const double RAD = PI/180.;

    date = dateTime.date();
    timeOfDay = dateTime.time();
    day = date.day();
    month = date.month();
    dayOfYear = date.dayOfYear();
    year = date.year();
    hour = timeOfDay.hour();
    minute = timeOfDay.minute();
    second = timeOfDay.second();

    totalTime = totalTimeSteps*timeStep;
    totalDays = totalTime*Time::conversionFactor(timeUnit, Time::Days);

    QDateTime beginning = QDateTime(QDate(year,1,1), QTime(), Qt::UTC);
    double secsPassed = beginning.secsTo(dateTime);
    double secsInYear = date.daysInYear()*24*60*60;
    dateAsReal = double(year) + secsPassed/secsInYear;

    double dec = -asin(sin(23.45*RAD)*cos(2*PI*(dayOfYear+10.)/365.));
    sinLD = sin(RAD*latitude)*sin(dec);
    cosLD = cos(RAD*latitude)*cos(dec);
    Q_ASSERT(TestNum::neZero(cosLD));
    double aob = sinLD/cosLD;
    dayLength = 12.*(1. + 2.*asin(aob)/PI);
    handleClockTick(hour + minute/60. + second/60./60.);
}

void Calendar::handleClockTick(double hour) {
    sinb = sinLD + cosLD*cos(2.*PI*(hour + 12.)/24.);
    if (sinb < 0.) sinb = 0.;
}

} //namespace

