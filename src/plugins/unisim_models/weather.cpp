/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cfloat>
#include <usbase/clock.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "../unisim_models/calendar.h"
#include "weather.h"

namespace UniSim{

Weather::Weather(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new PullVariable<double>("Tavg", &Tavg, this,
    "Daily average temperature calculated read from records file "
    "or calculated as the daily average of minimum and maximum temperature");
}

void Weather::initialize() {
    Model *records = seekOneChild<Model*>("records");
    PullVariable<double> *pullAvg = records->peekOneChild<PullVariable<double>*>("Tavg");
    PullVariable<double> *pullMin = records->peekOneChild<PullVariable<double>*>("Tmin");
    PullVariable<double> *pullMax = records->peekOneChild<PullVariable<double>*>("Tmax");

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

