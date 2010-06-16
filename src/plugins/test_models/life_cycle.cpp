/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <usbase/pull_variable.h>
#include "life_cycle.h"
#include "life_stage.h"

using namespace UniSim;

namespace test {

LifeCycle::LifeCycle(UniSim::Identifier name, QObject *parent)
	: Model(name,parent) 
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
    new PullVariable("contents", &sum, this, "description");
}

void LifeCycle::initialize()
{
	stages.clear();
	for (int i = 0; i < children().size(); ++i) {
		LifeStage* stage = dynamic_cast<LifeStage*>(children().at(i));
		if (stage) {
			stages.append(stage);
		}
	}
}

void LifeCycle::reset()
{
	sum = 0;
}

void LifeCycle::update()
{
    //cout << "LifeCycle::update() A\n";
    if (stages.size() > 0) {
        stages[0]->update();
        sum = stages[0]->pullVariable("contents");
		for (int i = 1; i < stages.size(); ++i) {
            stages[i]->pushVariable("input", stages[i-1]->pullVariable("output"));
			stages[i]->update();
            sum += stages[i]->pullVariable("contents");
		}
	}
    //cout << "LifeCycle::update() Z\n";
}

} //namespace
