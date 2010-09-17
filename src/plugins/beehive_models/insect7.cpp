/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/utilities.h>
#include "../unisim_models/stage.h"
#include "insect7.h"

using namespace UniSim;


namespace beehive{

Insect7::Insect7(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("initEggs", &initEggs, 30., this, "Initial number of insect eggs");
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Insect7::initialize() {
    // Find stages
    egg = seekOneChild<Model*>("egg");
    stages = seekChildren<Stage*>("*");
    Model *adult = seekOneChild<Model*>("adult");
    // Find submodels
    fecundity = adult->seekOneChild<Model*>("fecundity");
    eggSurvival = egg->seekOneChild<Model*>("survival");
}

void Insect7::reset() {
    egg->pushVariable("inflow", initEggs);
}

void Insect7::update() {
    // Development through stages
    stages[0]->deepUpdate();
    for (int i = 1; i < stages.size(); ++i) {
        stages[i]->pushVariable("inflow", stages[i-1]->pullVariable<double>("outflow"));
        stages[i]->deepUpdate();
    }

    // Reproduction
    egg->pushVariable("inflow", fecundity->pullVariable<double>("eggsLaid"));

    // Apply egg survival as finite growth rate which must be > 0
    double fgr = eggSurvival->pullVariable<double>("survival");
    if (fgr == 0.)
        fgr = 1e-6;
    egg->pushVariable("growthRate", fgr);
}

} //namespace

