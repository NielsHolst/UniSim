/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "ambrosia_time.h"
#include "publish.h"

using namespace UniSim;

namespace ambrosia{

PUBLISH(AmbrosiaTime)

AmbrosiaTime::AmbrosiaTime(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
    new Parameter<double>("L0", &L0, 14.5, this, "desc");
    new Parameter<double>("alfa", &alfa, 50., this, "desc");
    new Parameter<double>("T0", &T0, 0.9, this, "desc");
    new Parameter<double>("Tmax", &Tmax, 31.7, this, "desc");

    new Variable<double>("dayLengthIndex", &dayLengthIndex, this, "desc");
    new Variable<double>("temperatureIndex", &temperatureIndex, this, "desc");
    new Variable<double>("step", &step, this, "desc");
    new Variable<double>("total", &total, this, "desc");
}

void AmbrosiaTime::initialize() {
    calendar = seekOne<Model*>("calendar");
    weather = seekOne<Model*>("weather");
}

void AmbrosiaTime::reset() {
    total = 0.;
}

void AmbrosiaTime::update() {
    double L = calendar->pullValue<double>("dayLength");
    double T = weather->pullValue<double>("Tavg");
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

