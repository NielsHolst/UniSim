/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "single_immigration.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(SingleImmigration)

SingleImmigration::SingleImmigration(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, Anumber, 10.);
     Input(double, Pnumber, 10.);
    Input(int, immigrationDay, 20.);
    Input(int, currentDay, 0.);
    Output(double, Fimmigrants);
    Output(double, Mimmigrants);
    Output(double, immigrants);
}

void SingleImmigration::reset() {
    Fimmigrants = 0.,
    Mimmigrants = 0.,
    immigrants = 0.;
}

void SingleImmigration::update() {
    double Fnumber= Pnumber*0.6,
           Mnumber= Pnumber*0.4;
//    if (currentDay == immigrationDay)
//        immigrants = number;
//    else
//        immigrants = 0.;

    Fimmigrants = (currentDay == immigrationDay) ? Fnumber : 0.;
    Mimmigrants = (currentDay == immigrationDay) ? Mnumber : 0.;
    immigrants = (currentDay == immigrationDay) ? Mnumber : 0.;
}

} //namespace

