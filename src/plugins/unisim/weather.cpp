/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cfloat>
#include <usbase/clock.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "calendar.h"
#include "publish.h"
#include "weather.h"

namespace UniSim{

PUBLISH(Weather)

Weather::Weather(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Variable<double>("Tavg", &Tavg, this,
    "Daily average temperature, either read from records file, "
    "or calculated as the daily average of minimum and maximum temperature. "
    "Must have a child model called @F {records}.");
}

void Weather::initialize() {
    Model *records = seekOneChild<Model*>("records");
    Variable<double> *pullAvg = records->peekOneChild<Variable<double>*>("Tavg");
    Variable<double> *pullMin = records->peekOneChild<Variable<double>*>("Tmin");
    Variable<double> *pullMax = records->peekOneChild<Variable<double>*>("Tmax");

    avgIsPresent = pullAvg;
    minMaxIsPresent = pullMin && pullMax;
    if (avgIsPresent) {
        ptrAvg = pullAvg->valuePtr();
    }
    if (minMaxIsPresent) {
        ptrMin = pullMin->valuePtr();
        ptrMax = pullMax->valuePtr();
    }
}

void Weather::reset() {
    update();
}

void Weather::update() {
    Tavg = avgIsPresent ? *ptrAvg :
           minMaxIsPresent ? (*ptrMin + *ptrMax)/2. : 0.;
}

} //namespace

