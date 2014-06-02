/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "log_logistic.h"
#include "publish.h"

using namespace UniSim;

namespace ecotox {

PUBLISH(LogLogistic)

/*! \class LogLogistic
 * \brief Log-logistic dose-response curve
 *
    The log-logistic dose-response curve described by
    @Math{y = y sub min - {(y sub max - y sub min)} over {1 + {x over ED50} sup {-slope}} }.
    The value of the dose x is given by the parameter _dose_. As for any other parameter,
    you can either set this to a fixed or a variable value. For _dose_ you most often want a
    variable value supplied from another model. If you set _dose_ to _pollen[pollenDensity]_ then
    the response will reflect current pollen density. Other possible measures include
    _pollen[stdPollenDensity_] and _pollen[toxinDensity]_, see Pollen.
*/

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

    new Variable<double>("value", &value, this,
                         "Response at current dose");
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
    return lower + (upper - lower)/(1. + pow((z/ED50), -slope));
}

} //namespace

