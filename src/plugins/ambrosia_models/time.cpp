/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "time.h"

using namespace UniSim;


namespace ambrosia{

Time::Time(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
    new Parameter<double>("L0", &L0, 14.5, this, "desc");
    new Parameter<double>("alfa", &alfa, 50., this, "desc");
    new Parameter<double>("T0", &T0, 0.9, this, "desc");
    new Parameter<double>("Tmax", &Tmax, 31.7, this, "desc");

    new PullVariable<double>("dayLengthIndex", &dayLengthIndex, this, "desc");
    new PullVariable<double>("temperatureIndex", &temperatureIndex, this, "desc");
    new PullVariable<double>("step", &step, this, "desc");
    new PullVariable<double>("total", &total, this, "desc");
}

void Time::initialize() {
    calendar = seekOne<Model*>("calendar");
    weather = seekOne<Model*>("weather");
}

void Time::reset() {
    total = 0.;
}

void Time::update() {
    double L = calendar->pullVariable<double>("dayLength");
    double T = weather->pullVariable<double>("Tavg");
    dayLengthIndex = L < L0 ? 1. : exp((L - L0)*log(1. - alfa/100.));
    if (T < T0)
        temperatureIndex = 0.;
    else if (T < Tmax)
        temperatureIndex = 1. - (Tmax - T)/(Tmax - T0);
    else
        temperatureIndex = 1.;

    step = dayLengthIndex*temperatureIndex;
    total += step;
}

} //namespace

