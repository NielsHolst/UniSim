/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <cmath>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "larva_survival.h"

using namespace UniSim;

namespace {

    // Return exp(-x), x>=0; also works for extreme values
    double NegExp(double x)  {
        const double MAX_EXP = -log(DBL_EPSILON);
        const double MIN_EXP = -log(1. - DBL_EPSILON);
        return (x > MAX_EXP) ? 0. : (x < MIN_EXP) ? 1. : exp(-x);
    }

}


namespace beehive{

LarvaSurvival::LarvaSurvival(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("demandRate", &demandRate, 2., this,
    "Demand rate (unit of pollen per larva per day");

    new Parameter<double>("attackRate", &attackRate, 0.9, this,
    "Attack rate (per unit of pollen per day");

    new PullVariable<double>("survival", &survival, this,
    "Survival [0;1] depending on supply/demand ratio for pollen");

    new PullVariable<double>("sdRatio", &sdRatio, this,
    "Supply/demand ratio for pollen");

    new PullVariable<double>("eaten", &eaten, this,
    "Pollen mass eaten per day");
}

void LarvaSurvival::initialize() {
    pollen = seekOne<Model*>("pollen");
    larva = seekOne<Model*>("larva");
}

void LarvaSurvival::reset() {
    eaten = 0;
    sdRatio = 1;
    survival = 1;
}

void LarvaSurvival::update() {
    double consumer = larva->pullVariable<double>("number");
    double ressource = pollen->pullVariable<double>("mass");
    double demand = consumer*demandRate;
    double attacked = ressource*attackRate;
    eaten = funcResponse(demand, attacked);
    sdRatio = survival = demand > 0. ? eaten/demand : 0;
    pollen->pushVariable("loss", eaten);
}

double LarvaSurvival::funcResponse(double demand, double attacked)
{
    if (demand < 0.)
        throw Exception("Demand < 0",   this);
    if (attacked < 0.)
        throw Exception("Attack rate < 0", this);
    if (demand < DBL_EPSILON || attacked < DBL_EPSILON)
        return 0.;
    double result = demand*(1. - NegExp(attacked/demand));
    bool imprecise = result > attacked;
    if (imprecise)
        result = attacked;
    return result;
}
} //namespace

