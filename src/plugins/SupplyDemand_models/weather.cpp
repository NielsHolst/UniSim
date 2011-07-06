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

using namespace UniSim;

namespace SupplyDemand{

Weather::Weather(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new PullVariable<double>("Tavg", &Tavg, this,
    "Daily average temperature calculated as the average of @F Tmin and @F Tmax ({@Degree}C)");
}

void Weather::initialize() {
    records = seekOneChild<Model*>("records");
}

void Weather::reset() {
    update();
}

void Weather::update() {
    double Tmin = records->pullVariable<double>("Tmin");
    double Tmax = records->pullVariable<double>("Tmax");
    Tavg = (Tmin + Tmax)/2.;
}

} //namespace

