/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "damage.h"
#include "growth.h"

using std::min;
using namespace UniSim;

namespace xpest {
	
Growth::Growth(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(radiation), "weather[irradiationMJ]");
    addParameter<double>(Name(lai), 2., "Crop leaf area index");
    addParameter<double>(Name(k), 0.65, "Crop Extinction coefficient");
    addParameter<double>(Name(rue), 1.19, "Radiation use efficieny (g/MJ)");
    addVariable<double>(Name(production), "Amount of assimilate produced (g/m2/day)");
    addVariable<double>(Name(loss), "Amount of assimilate lost to pests (g/m2/day)");
    addVariable<double>(Name(gain), "Net gain of assimilate = @F production - @F loss (g/m2/day)");
    addVariable<double>(Name(rueReduction), "Reduction in RUE [0;1]");
    addVariable<double>(Name(laiReduction), "Reduction in affective LAI [0;1]");
}

void Growth::amend() {
    assimilateDemands = seekMany<Damage*>("damages/assimilateDemand/*");
    assimilateDiversions = seekMany<Damage*>("damages/assimilateDiversion/*");
    laiReductions = seekMany<Damage*>("damages/lai/*");
    rueReductions = seekMany<Damage*>("damages/rue/*");
}

void Growth::reset() {
    laiReduction = rueReduction = production = loss = gain = 0.;
}

void Growth::update() {
//    rate = radiation*rue*(1. - exp(-k*lai));
    setRueReduction();
    setLaiReduction();
    production = radiation*rue*(1.-rueReduction)*(1. - exp(-k*lai*(1.-laiReduction)));
    loss = production*assimilateReduction(production);
    gain = production - loss;
}

namespace {
    double demandFactor(const QList<Damage*> &demands, double production) {
        double factor = 1.;
        if (production > 0.) {
            for (int i = 0; i < demands.size(); ++i) {
                double demand = demands.at(i)->pullValue<double>("damage");
                double diversion = min(demand/production, 1.);
                factor *= 1. - diversion;
            }
        }
        return factor;
    }

    double diversionFactor(const QList<Damage*> &diversions) {
        double factor = 1.;
        for (int i = 0; i < diversions.size(); ++i) {
            double diversion = diversions.at(i)->pullValue<double>("damage");
            factor *= 1. - diversion;
        }
        return factor;
    }
}

void Growth::setRueReduction() {
    rueReduction = 1. - diversionFactor(rueReductions);
}

void Growth::setLaiReduction() {
    laiReduction = 1. - diversionFactor(laiReductions);
}

double Growth::assimilateReduction(double production) {
    return 1. - demandFactor(assimilateDemands, production)
                *diversionFactor(assimilateDiversions);
}

} //namespace

