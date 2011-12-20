/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/push_variable.h>
#include "log_logistic.h"

using namespace UniSim;

namespace ecotox {

LogLogistic::LogLogistic(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("dose", &dose, 0., this,
                          "Dose for which response will be calculated. You can use this also as a push variable");
    new Parameter<double>("lower", &lower, 0., this,
                          "Asymptotic response at low dose");
    new Parameter<double>("upper", &upper, 1., this,
                          "Asymptotic response at high dose");
    new Parameter<double>("ED50", &ED50, 10., this,
                          "Dose giving average response @F {(lower + upper)/2}");
    new Parameter<double>("slope", &slope, 2., this,
                          "Slope of sigmoid curve. The curve is rising from @F lower to @F upper, if @F slope is positive."
                          "And, the curve is falling from @F upper to @F lower, if @F slope is negative");

    new PullVariable<double>("value", &value, this,
                             "Response at current dose");
    new PushVariable<double>("dose", &dose, this,
                             "Dose for which response will be calculated");
}

void LogLogistic::initialize() {
}

void LogLogistic::reset() {
    value = f(dose = 0.);
}

void LogLogistic::update() {
    value = f(dose);
}

double LogLogistic::f(double z) {
    if (slope == 0.) return (upper - lower)/2.;
    if (z == 0) return (slope > 0.) ? lower : upper;
    return lower + (upper - lower)/(1. + pow((dose/ED50), -slope));
}

} //namespace

