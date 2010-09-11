/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "egg_survival.h"

using namespace UniSim;


namespace beehive{

EggSurvival::EggSurvival(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("a", &a, -0.00706, this, "For parabolic equation");
    new Parameter<double>("b", &b, 0.370, this, "For parabolic equation");
    new Parameter<double>("c", &c, -4.12, this, "For parabolic equation");
    new Parameter<double>("Tlow", &Tlow, 21., this,
    "The parabola is extrapolated horizontally below this temperature");
    new PullVariable<double>("survival", &survival, this,
    "Survival [0;1] depending on current temperature");
}

void EggSurvival::initialize() {
    weather = seekOne<Model*>("weather");
}

void EggSurvival::reset() {
    survival = 1;
}

void EggSurvival::update() {
    double temp = weather->pullVariable<double>("Tavg");
    survival = calcSurvival(temp);
}

double EggSurvival::calcSurvival(double temp) const {
    if (temp < Tlow)
        return calcSurvival(Tlow);
    double surv =  a*temp*temp + b*temp + c;
    return surv < 0 ? 0. : surv;
}
} //namespace

