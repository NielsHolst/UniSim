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
#include "calendar.h"

using namespace std;

namespace UniSim{

Calendar::Calendar(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setState("daysTotal", &daysTotal);
    setState("dayInYear", &dayInYear);
    setState("dayOfYear", &dayInYear);
    setState("day", &day);
    setState("month", &month);
    setState("year", &year);
    setState("dayLength", &dayLength);
    setState("sinLD", &sinLD);
    setState("cosLD", &cosLD);
}

void Calendar::initialize()
{
    setParameter("firstDate", &firstDate, QDate());
    setParameter("latitude", &latitude, 52.);
    setParameter("followers", &followersAsString, QString());

    followers.clear();
    QStringList followersAsStrings = decodeSimpleList(followersAsString, "Calendar::initialize");
    for (int i = 0; i < followersAsStrings.size(); ++i)
        followers.append(seekOne<Model*>(followersAsStrings.value(i)));
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
            throw Exception("Follower of calendar must have parameter firstDatw");
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
            flw->changeParameter("firstDate", firstDate);
        }
        else if (firstDate < flwFirstDate) {
            firstDate = flwFirstDate;
            break;
        }
        else if (firstDate > flwFirstDate) {
            int daysTo = flwFirstDate.daysTo(firstDate);
            for (int i = 0; i < daysTo; ++i)
                flw->deepUpdate();
        }
    }
}

void Calendar::update()
{
    const double RAD = PI/180.;

    daysTotal += 1.;
    date = date.addDays(1);
    day = date.day();
    month = date.month();
    dayInYear = QDate(date.year(), 1, 1).daysTo(date) + 1;
    year = date.year() + double(dayInYear)/date.daysInYear();

    double dec = -asin(sin(23.45*RAD)*cos(2*PI*(dayInYear+10.)/365.));
    sinLD = sin(RAD*latitude)*sin(dec);
    cosLD = cos(RAD*latitude)*cos(dec);
    dayLength = 12.*(1.+2.*asin((-sin(-4.*RAD)+sinLD)/cosLD)/PI);

}


double Calendar::sinb(double hour) const {
    double sb = sinLD + cosLD*cos(2.*PI*(hour + 12.)/24.);
    return sb > 0. ? sb : 0.;
}

} //namespace

