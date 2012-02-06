/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include "../unisim_models/stage.h"
#include "insect8.h"

using namespace UniSim;


namespace beehive{

Insect8::Insect8(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("initEggs", &initEggs, 30., this, "Initial number of insect eggs");
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Insect8::initialize() {
    // Find stages
    egg = seekOneChild<Model*>("egg");
    larva = seekOneChild<Model*>("larva");
    stages = seekChildren<Stage*>("*");
    Model *adult = seekOneChild<Model*>("adult");
    // Find submodels
    fecundity = adult->seekOneChild<Model*>("fecundity");
    eggSurvival = egg->seekOneChild<Model*>("survival");
    larvaSurvival = larva->seekOneChild<Model*>("survival");
}

void Insect8::reset() {
    egg->pushVariable("inflow", initEggs);
}

void Insect8::update() {
    // Development through stages
    stages[0]->deepUpdate();
    for (int i = 1; i < stages.size(); ++i) {
        stages[i]->pushVariable("inflow", stages[i-1]->pullVariable<double>("outflow"));
        stages[i]->deepUpdate();
    }

    // Reproduction
    egg->pushVariable("inflow", fecundity->pullVariable<double>("eggsLaid"));

    // Survival
    applySurvival(egg, eggSurvival);
    applySurvival(larva, larvaSurvival);
}

void Insect8::applySurvival(Model *model, Model *survival) {
    // Apply survival as finite growth rate which must be > 0
    double fgr = survival->pullVariable<double>("survival");
    if (fgr == 0.)
        fgr = 1e-6;
    model->pushVariable("growthRate", fgr);
}

} //namespace

