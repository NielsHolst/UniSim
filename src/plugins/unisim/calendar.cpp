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
#include <usbase/utilities.h>
#include "calendar.h"
#include "publish.h"

using namespace std;

namespace UniSim{

PUBLISH(Calendar)

Calendar::Calendar(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(latitude), 52., "Latitude of simulated system (degrees)");
    addParameter<double>(Name(longitude), 11., "Longitude of simulated system (degrees)");

    addParameter<int>(Name(timeZone), 1,
    "Time zone in hours relative to GMT, e.g., Copenhagen has @F timeZone = 1");

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

    Input(int, sample, 1);

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
    addVariable<double>(Name(azimuth), "Azimuth of the sun relative to north (degrees)");
}

void Calendar::initialize() {
    connect(clock(), SIGNAL(tick(double)), this, SLOT(handleClockTick(double)));
    timeUnit = Time::charToUnit(timeUnitAsChar);
    timeStepSecs = timeStep*Time::conversionFactor(timeUnit, Time::Seconds);
}

void Calendar::reset() {
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
    updateAzimuth();
}

//! Azimuth is 90 at noon, zero at sunset and sunrise, and -90 at midnight
/* See
    http://www.esrl.noaa.gov/gmd/grad/solcalc/calcdetails.html
    http://www.jgiesen.de/astro/suncalc/calculations.htm
*/
void Calendar::updateAzimuth() {
    // First, the fractional year y is calculated, in radians.
    double y = 2*PI*(dayOfYear-1+(hour-12.)/24)/365.;

    //From y, we can estimate the equation of time (in minutes) and the solar declination angle (in radians).
    double eqtime =
    229.18*( 0.000075+0.001868*cos(y)-0.032077*sin(y)-0.014615*cos(2*y)-0.040849*sin(2*y) );

    double declin = 0.006918-0.399912*cos(y)+0.070257*sin(y)
            -0.006758*cos(2*y)+0.000907*sin(2*y)
            -0.002697*cos(3*y)+0.00148*sin(3*y);

    // Next, the true solar time is calculated in the following two equations. First the time offset is
    // found, in minutes, and then the true solar time, in minutes.
    double timeOffset = eqtime - 4*longitude + 60*timeZone;
    // where eqtime is in minutes, longitude is in degrees, timezone is in hours from UTC

    // True solar time
    double tst = hour*60 + + minute + second/60. + timeOffset;

    // The solar hour angle, in radians, is:
    double ha = (tst/4 - 180)*RAD;

    // The solar zenith angle (phi) can then be found from the following equation:
    double lat = latitude*RAD;
    double cosPhi = sin(lat)*sin(declin) + cos(lat)*cos(declin)*cos(ha);
    double phi = acos(cosPhi);

    // And the solar azimuth is:
    double az1 = -(sin(lat)*cos(phi)-sin(declin))/(cos(lat)*sin(phi));
    double az2 = acos(az1)*DEGREES;
    azimuth = az2-90.;
}

void Calendar::handleClockTick(double hour) {
    sinb = sinLD + cosLD*cos(2.*PI*(hour + 12.)/24.);
    if (sinb < 0.) sinb = 0.;
}

} //namespace

