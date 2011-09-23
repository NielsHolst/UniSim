/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <QMessageBox>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "maize.h"

using namespace UniSim;

namespace SupplyDemand {

Maize::Maize(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("alpha", &alpha, 21.54, this,
                          "Pollen phenology shape parameter");
    new Parameter<double>("beta", &beta, 1.868, this,
                          "Pollen phenology shape parameter");
    new Parameter<double>("c", &c, 1., this,
                          "Pollen phenology compression. "
                          "@F c < 1 prolongs the periods of pollen deposition. "
                          "@F c > 1 shortens the period.");
    new Parameter<double>("distance", &distance, 1., this,
                          "Distance to maize field (m)");
    new Parameter<double>("minDist", &minDist, 0.3, this,
                          "Minimum distance of pollen dispersion model (m)");
    new Parameter<double>("halfDist", &halfDist, 3.55, this,
                          "Half-distance of pollen dispersion (m)");
    new Parameter<double>("halfLife", &halfLife, 14., this,
                          "Half-life of pollen (days)");
    new Parameter<QDate>("dateMax", &dateMax, QDate(2011,7,31), this,
                          "Date when pollen deposition peaks");
    new Parameter<double>("dateShift", &dateShift, 0., this,
                          "Shift dateMax @F dateShift days");
    new Parameter<double>("Pmax", &Pmax, 121, this,
                          "Max. pollen deposition rate at zero distance (pollen per cm @Sup2");
    new Parameter<double>("btQuantum", &btQuantum, 0.1, this,
                          "Quantum of Bt toxin per pollen (pg)");
    new PullVariable<double>("pollenDepositionRate", &pollenDepositionRate, this, "Pollen deposition rate (per day per cm @Sup {2})");
    new PullVariable<double>("pollenDepositionTotal", &pollenDepositionTotal, this, "Pollen deposition total (per cm @Sup {2})");
    new PullVariable<double>("pollen", &pollen, this, "Current pollen density (per cm @Sup {2})");
    new PullVariable<double>("toxin", &toxin, this, "Current toxin density (pg\"/\"cm @Sup {2})");
}

void Maize::initialize() {
    Model *calendar = seekOne<Model*>("calendar");
    dayOfYear = calendar->pullVariablePtr<int>("dayOfYear");
}

void Maize::reset() {
    pollenDepositionRate = pollenDepositionTotal = 0.;
    pollen = toxin = 0.;
    if (distance < minDist)
        distance = minDist;
    tmax = dateMax.dayOfYear() + dateShift;
    survivalRate = pow(0.5, 1./halfLife);
}

void Maize::update() {
    pollenDepositionRate = Pdist()*Ptime(*dayOfYear);
    pollenDepositionTotal += pollenDepositionRate;
    pollen = pollen*survivalRate + pollenDepositionRate;
    toxin = pollen*btQuantum;
}

double Maize::Pdist() {
    return Pmax/pow(distance,log(2)/log(halfDist));
}

double Maize::Ptime(int t) {
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

