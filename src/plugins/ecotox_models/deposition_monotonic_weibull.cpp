/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "deposition_monotonic_weibull.h"

using namespace UniSim;


namespace ecotox {

DepositionMonotonicWeibull::DepositionMonotonicWeibull(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("alpha", &alpha, 21.54, this,
                          "Pollen phenology shape parameter");
    new Parameter<double>("beta", &beta, 1.868, this,
                          "Pollen phenology shape parameter");
    new Parameter<double>("c", &c, 1., this,
                          "Pollen phenology compression. "
                          "@F c < 1 prolongs the period of pollen deposition. "
                          "@F c > 1 shortens the period.");
    new Parameter<QDate>("dateMax", &dateMax, QDate(2011,7,31), this,
                          "Date when pollen deposition peaks");
    new Parameter<double>("dateShift", &dateShift, 0., this,
                          "Shift @F dateMax by this number of days. Usefull for stochastic modelling.");


    new PullVariable<double>("value", &value, this,
                           "Deposition rate (0..1)");
    new PullVariable<double>("total", &total, this,
                          "Accumulated deposition total (0..1)");
}

void DepositionMonotonicWeibull::initialize() {
    Model *calendar = seekOne<Model*>("calendar");
    dayOfYear = calendar->pullVariablePtr<int>("dayOfYear");
}

void DepositionMonotonicWeibull::reset() {
    value = total = 0.;
    tmax = dateMax.dayOfYear() + dateShift;
}

void DepositionMonotonicWeibull::update() {
    value = Ptime(*dayOfYear);
    total += value;
}

double DepositionMonotonicWeibull::Ptime(int t) {
    double m = (beta - 1.)/beta;
    double t0 = tmax - alpha*pow(m,1./beta);
    double tp = t0 + c*(t - t0);
    if (tp <= t0)
        return 0.;
    double n = (tp - tmax)/alpha;
    double mp = pow(m, 1./beta);
    double a = beta*exp(-pow(n+mp, beta))*pow(alpha*mp, beta)*pow(n+mp, beta-1.);
    double b = pow(alpha, beta+1.)*pow(m,(beta*m+1.)/beta);
    return c*a/b;
}

} //namespace

