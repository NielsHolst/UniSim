/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QString>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "insect.h"
#include "stage.h"

namespace UniSim{
	
Insect::Insect(Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Update,ChildrenNot);
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this,
                          "Proportion females");
    new Parameter<double>("eggProduction", &eggProduction, 100., this,
                          "Lifetime egg production of one female");
    new PullVariable<double>("newAdults", &newAdults, this, "Number of individuals entering adult stage");
    new PullVariable<double>("lostAdults", &lostAdults, this, "Number of individuals dead from senescence");

}

void Insect::initialize() {
    Model *lifeCycle = seekOneChild<Model*>("lifeCycle");
    stages = lifeCycle->seekChildren<Stage*>("*");
    if (stages.size() < 3)
        throw Exception("There must be at least two Stage models in lifeCycle", this);
    eggLayingPhase = seekOneChild<Model*>("eggLayingPhase");
    hibernatingAdult = seekOneChild<Model*>("hibernatingAdult");
}

void Insect::reset() {
    newAdults = lostAdults = 0.;
}

void Insect::update() {
    reproduce();
    stages[0]->deepUpdate();
    double transfer = stages[0]->pullVariable<double>("outflow");
    int n = stages.size();
    for (int i = 0;  i < n; ++i) {
        stages[i]->pushVariable<double>("inflow", transfer);
        stages[i]->deepUpdate();
        if (i == n-1)
            newAdults = transfer;
        transfer = stages[i]->pullVariable<double>("outflow");
    }
    lostAdults = transfer;
}

void Insect::reproduce() {
    double eggsToLay = newAdults*sexRatio*eggProduction;
    eggLayingPhase->pushVariable<double>("inflow", eggsToLay);
    eggLayingPhase->deepUpdate();
    double eggsLaid = eggLayingPhase->pullVariable<double>("outflow");
    stages[0]->pushVariable<double>("inflow", eggsLaid);
}

} //namespace



