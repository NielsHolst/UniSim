/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include <usbase/pull_variable.h>
#include <usbase/test_num.h>
#include "calendar.h"

using namespace std;

namespace UniSim{

Calendar::Calendar(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<QDate>("firstDate", &firstDate, QDate(), this, "Initial date of simulation");
    new Parameter<double>("latitude", &latitude, 52., this, "Latitude of simulated system");
    new Parameter<QString>("followers", &followersAsString, QString(), this,
    "A single name, or list of names, denoting those objects that follow "
    "(are synchronized with) the calendar date. "
    "Commonly the @F weather object is set as a follower");
    new Parameter<int>("stepsPerDay", &stepsPerDay, 1, this, "Time steps per days of simulation. "
        "Usually this should be set according to the time resolution of the @F weather object. "
        "Daily readings for example corresponds to a @F stepsPerDay of @F {1} and hourly "
        "readings to a @F stepsPerDay of @F {24}");

    new PullVariable<QDate>("date", &date, this, "Current date");
    new PullVariable<int>("daysTotal", &daysTotal, this, "Days total since beginning of simulation");
    new PullVariable<int>("dayInYear", &dayInYear, this, "Day number in year, also known as Julian day");
    new PullVariable<int>("dayOfYear", &dayInYear, this, "Synonymous with @F {dayInYear}");
    new PullVariable<int>("day", &day, this, "Current day in month (1..31)");
    new PullVariable<int>("month", &month, this, "Current montn (1..12)");
    new PullVariable<double>("year", &year, this, "Current year");
    new PullVariable<double>("dayLength", &dayLength, this, "Current day length (hours)");
    new PullVariable<double>("sinb", &sinb, this, "Sine of sun elevation, updated by the @F tick event of the @F clock object");
    new PullVariable<double>("sinLD", &sinLD, this, "Intermediate variable in astronomic calculations");
    new PullVariable<double>("cosLD", &cosLD, this, "Intermediate variable in astronomic calculations");
}

void Calendar::initialize()
{
    if (stepsPerDay <= 0)
        throw Exception("Calendar parameter 'stepPerDays must be > 0");

    followers.clear();
    QStringList followersAsStrings = decodeSimpleList(followersAsString, this);
    for (int i = 0; i < followersAsStrings.size(); ++i)
        followers.append(seekOne<Model*>(followersAsStrings.value(i)));

    connect(clock(), SIGNAL(tick(double)), this, SLOT(handleClockTick(double)));
}

void Calendar::reset() {
    QString msg;
    getFollowerFirstDates();

    switch (firstDateDiagnose()) {
        case NoneNoFollowers:
            msg = "Calendar misses parameter firstDate";
            break;
        case NoneWithFollowersWithFirstDate:
            firstDate = followerFirstDates.value(0);
            break;
        case NoneWithFollowersWithoutFirstDate:
            msg = "Calendar misses parameter firstDate";
            break;
        case PresentNoFollowers:
            // nothing to do
            break;
        case PresentWithFollowers:
            synchronizeWithFollowers();
            break;
        case FollowersConflicting:
            msg = "Conflicting values of firstDate in followers of calendar: " +
                  followersAsString;
            break;
        default:
            Q_ASSERT(false);
    }
    if (!msg.isEmpty())
        throw Exception(msg);

    date = firstDate.addDays(-1);
    daysTotal = 0;
    dateTime.setDate(date);
    updateDerived();
}

void Calendar::getFollowerFirstDates() {
    followerFirstDates.clear();
    for (int i = 0; i < followers.size(); ++i) {
        QDate date;
        try {
            date = followers[i]->parameter<QDate>("firstDate");
            if (!date.isNull())
                followerFirstDates.append(date);
        }
        catch (Exception &ex) {
            throw Exception("Follower of calendar must have parameter firstDate");
        }
    }
}

Calendar::FirstDateDiagnose Calendar::firstDateDiagnose() {
    if (!sameFollowerFirstDates())
        return FollowersConflicting;

    if (firstDate.isNull()) {
        if (followers.isEmpty())
            return NoneNoFollowers;
        else if (followerFirstDates.isEmpty())
            return NoneWithFollowersWithoutFirstDate;
        else
            return NoneWithFollowersWithFirstDate;
    }
    else {
        if (followers.isEmpty())
            return PresentNoFollowers;
        else
            return PresentWithFollowers;
    }

}

bool Calendar::sameFollowerFirstDates() {
    for (int i = 1; i < followerFirstDates.size(); ++i) {
        if (followerFirstDates.value(0) != followerFirstDates.value(i))
            return false;
    }
    return true;
}

void Calendar::synchronizeWithFollowers() {
    for (int i = 0; i < followers.size(); ++i) {
        Model *flw = followers[i];
        QDate flwFirstDate = flw->parameter<QDate>("firstDate");

        if (flwFirstDate.isNull()) {
            Parameter<QDate> *p = flw->seekOneChild<Parameter<QDate> *>("firstDate");
            p->setValue(firstDate);
        }
        else if (firstDate < flwFirstDate) {
            firstDate = flwFirstDate;
            break;
        }
        else if (firstDate > flwFirstDate) {
            int daysTo = flwFirstDate.daysTo(firstDate);
            flw->deepReset();
            for (int i = 0; i < daysTo; ++i)
                flw->deepUpdate();
        }
    }
}

void Calendar::update() {
    daysTotal += 1./stepsPerDay;
    dateTime = dateTime.addSecs(24*60*60/stepsPerDay);
    date = dateTime.date();
    updateDerived();
}

void Calendar::updateDerived() {
    const double RAD = PI/180.;

    day = date.day();
    month = date.month();
    dayInYear = QDate(date.year(), 1, 1).daysTo(date) + 1;
    year = date.year() + double(dayInYear)/date.daysInYear();

    double dec = -asin(sin(23.45*RAD)*cos(2*PI*(dayInYear+10.)/365.));
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

