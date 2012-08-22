/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
/* Day length and solar elevation equations copied from the FORTRAN code of Kroppf &
** Laar (1993). Modelling Crop-Weed Interactions. CAB International.
*/
#include <iostream>
#include <QMessageBox>
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
    new Parameter<double>("latitude", &latitude, 52., this, "Latitude of simulated system");

    new Parameter<QDate>("initialDate", &initialDate, QDate(2000,1,1), this,
    "Initial date of simulation. "
    "You should perform a @F deepReset on your @F Calendar object after pushing a new "
    "value to @F {initialDate}.");

    new Parameter<QTime>("initialTimeOfDay", &initialTimeOfDay, QTime(0,0,0), this,
    "Initial time of day of simulation. Default is midnight. "
    "You should perform a @F deepReset on your @F Calendar object after pushing a new "
    "value to @F {initialDate}.");

    new Parameter<int>("timeStep", &timeStep, 1, this,
    "Duration of one integration time step in units determined by @F {timeUnit}");

    new Parameter<char>("timeUnit", &timeUnitAsChar, 'd', this,
    "Time unit of @F {timeStep}: s)econds, m)inutes, h)ours, d)ays or y)ears.");

    new Parameter<int>("timeStepOffset", &timeStepOffset, -1, this,
    "The first values are reported to plot and table outputs after one time step. "
    "With the default value of -1 for @F timeStepOffset, the first output will occur "
    "at time zero, defined by @F initialDay and @F {initialTimeOfDay}. "
    "Often this is what is intuitively expected. With a @F timeStepOffset value of zero "
    "the first output will occur one time step after time zero.");

    new Variable<QDate>("date", &date, this, "Current date");
    new Variable<QTime>("timeOfDay", &timeOfDay, this, "Current time of day");
    new Variable<QDateTime>("dateTime", &dateTime, this, "Current date and time");
    new Variable<int>("totalTimeSteps", &totalTimeSteps, this,
    "Total number of time steps performed since beginning of simulation");
    new Variable<int>("totalTime", &totalTime, this,
    "Total time, in units determined by @F {timeUnit}. passed since beginning of simulation");
    new Variable<double>("totalDays", &totalDays, this,
    "Total days passed since beginning of simulation");
    new Variable<int>("dayOfYear", &dayOfYear, this, "Day number in year, also known as Julian day");
    new Variable<int>("day", &day, this, "Current day in month (1..31)");
    new Variable<int>("month", &month, this, "Current month (1..12)");
    new Variable<int>("year", &year, this, "Current year");
    new Variable<int>("hour", &hour, this, "Current hour of the day (0..23)");
    new Variable<int>("minute", &minute, this, "Current minute of the hour (0..59)");
    new Variable<int>("second", &second, this, "Current second of the minute (0..59)");


    new Variable<double>("dateAsReal", &dateAsReal, this, "Date as a real number measured in years");
    new Variable<double>("dayLength", &dayLength, this, "Current day length (hours)");
    new Variable<double>("sinb", &sinb, this, "Sine of sun elevation, updated by the @F tick event of the @F clock object");
    new Variable<double>("sinLD", &sinLD, this, "Intermediate variable in astronomic calculations, updated by the @F tick event of the @F clock object");
    new Variable<double>("cosLD", &cosLD, this, "Intermediate variable in astronomic calculations, updated by the @F tick event of the @F clock object");
}

void Calendar::initialize() {
    connect(clock(), SIGNAL(tick(double)), this, SLOT(handleClockTick(double)));
}

void Calendar::reset() {
    timeUnit = Time::charToUnit(timeUnitAsChar);
    dateTime = QDateTime(initialDate, initialTimeOfDay, Qt::UTC);
    dateTime = dateTime + Time(timeStep*timeStepOffset, timeUnit);
    totalTimeSteps = 0;
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
    totalDays = totalTime/Time::conversionFactor(timeUnit, Time::Days);

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
}

void Calendar::handleClockTick(double hour) {
    sinb = sinLD + cosLD*cos(2.*PI*(hour + 12.)/24.);
    if (sinb < 0.) sinb = 0.;
}

} //namespace

