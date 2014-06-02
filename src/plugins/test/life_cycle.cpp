/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <usbase/variable.h>
#include "life_cycle.h"
#include "life_stage.h"
#include "publish.h"

using namespace UniSim;

namespace test {

PUBLISH(LifeCycle)

LifeCycle::LifeCycle(UniSim::Identifier name, QObject *parent)
	: Model(name,parent) 
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
    new Variable<double>("contents", &sum, this, "description");
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
        sum = stages[0]->pullValue<double>("contents");
		for (int i = 1; i < stages.size(); ++i) {
            stages[i]->pushValue("input", stages[i-1]->pullValue<double>("output"));
			stages[i]->update();
            sum += stages[i]->pullValue<double>("contents");
		}
	}
    //cout << "LifeCycle::update() Z\n";
}

} //namespace
