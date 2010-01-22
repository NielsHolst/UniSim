/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
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
    setState("year", &year);
    setState("dayLength", &dayLength);
    setState("sinLD", &sinLD);
    setState("cosLD", &cosLD);
}

void Calendar::initialize()
{
    setParameter("startingDayInYear", &startingDayInYear, 1.);
    setParameter("latitude", &latitude, 52.);
}

void Calendar::reset() {
    daysTotal = 0;
    dayInYear = startingDayInYear - 1;
}

void Calendar::update()
{
    const double RAD = PI/180.;

    daysTotal += 1.;
    year = daysTotal/365.;
    dayInYear += 1.;
    if (dayInYear > 365.) dayInYear -= 365.;

    double dec = -asin(sin(23.45*RAD)*cos(2*PI*(dayInYear+10.)/365.));
    sinLD = sin(RAD*latitude)*sin(dec);
    cosLD = cos(RAD*latitude)*cos(dec);
    dayLength = 12.*(1.+2.*asin((-sin(-4.*RAD)+sinLD)/cosLD)/PI);


    QString s;
    QTextStream text(&s);
    //text << "calendar::update\n" << dayLength;
    //QMessageBox::information(0,"Test", s);
}


double Calendar::sinb(double hour) const {
    double sb = sinLD + cosLD*cos(2.*PI*(hour + 12.)/24.);
    return sb > 0. ? sb : 0.;
}
} //namespace

