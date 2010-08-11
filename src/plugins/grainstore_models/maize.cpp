/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "maize.h"

using namespace UniSim;

namespace grainstore{

Maize::Maize(UniSim::Identifier name, QObject *parent)
    : Model(name, parent) {
    new PullVariable<double>("lossPct", &injury[LossPct], this, "desc");
    new PullVariable<double>("damagePct", &injury[DamagePct], this, "desc");
    new PullVariable<double>("ptSD", &ptSD[LossPct], this, "desc");

    paramText[LossPct] = "loss";
    paramText[DamagePct] = "damage";
}

void Maize::initialize() {
    setParameter("backgroundLossRate", &backgroundRate[LossPct], 0.0379, "desc");
    setParameter("backgroundDamageRate", &backgroundRate[DamagePct], 0.212, "desc");

    // work in progress
    //ptAdult = findOne<Model*>("prostephanus/development/adult");
}

void Maize::reset() {
    injury[LossPct] = injury[DamagePct] = 0.;
}

void Maize::update() {
    updateInjury(LossPct);
    updateInjury(DamagePct);
}

void Maize::updateInjury(InjuryType type) {
    // work in progress
    /*
    double density = ptAdult->state("number");
    double demandRate = ptAdult->parameter<double>(paramText[type] + "DemandRate");
    double ptDemand = density*demandRate;

    double searchRate = ptAdult->parameter<double>(paramText[type] + "SearchRate");

    double ptSupply = GBFuncResp(ptDemand, searchRate*(100. - injury[type]));
    ptSD[type] = ptDemand > 0. ? ptSupply/ptDemand : 0.;

    double othersSupply = backgroundRate[type]*(1. - injury[type]/100.);

    double injuryRate = ptSupply + othersSupply;
    injury[type] = (injury[type] + injuryRate) > 100. ?
                   100. - injury[type] :
                   injuryRate;
    */
}

} //namespace

