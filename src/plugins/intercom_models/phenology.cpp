/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "phenology.h"

using namespace UniSim;

namespace intercom{

Phenology::Phenology(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Phenology::reset() {
    stages[0]->pushVariable<double>("inflow", 1.);
}

void Phenology::initialize() {
    stages = seekChildren<Model*>("*");
}

void Phenology::update() {
    // Development through stages
    stages[0]->deepUpdate();
    for (int i = 1; i < stages.size(); ++i) {
        stages[i]->pushVariable("inflow", stages[i-1]->pullVariable<double>("outflow"));
        stages[i]->deepUpdate();
    }
}

} //namespace

