/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "community.h"
#include "plant.h"

using namespace UniSim;


namespace conductance {

Community::Community(UniSim::Identifier name, QObject *parent)
    : Model(name, parent), phase(Unlimited)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
    new PullVariable("sum_sz", &sum_sz, this, "description");
}

void Community::initialize() {
    setParameter("dt", &dt, 1., "description");
    plants = seekChildren<Plant*>("*");
	if (plants.isEmpty())
		throw Exception("Community has no plants");
	else if (plants.size() > 2)
        throw Exception("Max. 2 plants are allowed in community");
}

void Community::reset() {
    smallest = -1;

    updateTotalCrownZoneArea();
    if (sum_sz > 1) {
        QString msg = "Total crown zone area must be < 1 m2 on day 0; sum_sz = " +
                      QString::number(sum_sz);
        throw Exception(msg);
    }
}

void Community::update() {
    int steps = (int) (1. + 1e-6)/dt;
    for (int i = 0; i < steps; ++i) {
        bool phaseChanged = true;
        while (phaseChanged) {
            updatePlants();
            updateTotalCrownZoneArea();
            Phase prevPhase = phase;
            if (phase == Unlimited)
                updateUnlimited();
            else if (phase == UnderCompression && plants.size() == 2)
                updateUnderCompression();
            phaseChanged = prevPhase != phase;
        }
    }
}

void Community::updatePlants() {
    for (int i = 0; i < plants.size(); ++i)
        plants[i]->updateByDt(dt);
}

void Community::updateTotalCrownZoneArea() {
    sum_sz = 0.;
    for (int i = 0; i < plants.size(); ++i)
        sum_sz += plants[i]->pullVariable("total_sz");
}
void Community::updateUnlimited() {
    if (sum_sz >= 1.) {
        smallest = (plants.size() == 1
                   || plants[1]->pullVariable("weight") > plants[0]->pullVariable("weight")) ? 0: 1;
        plants[smallest]->changePhase(UnderCompression);
        phase = UnderCompression;
    }
}

void Community::updateUnderCompression() {
    Q_ASSERT(smallest>=0 && smallest<=1);
    if (plants[smallest]->pullVariable("Lz") >= plants[1-smallest]->pullVariable("Lz")) {
        plants[0]->changePhase(WeightProportional);
        plants[1]->changePhase(WeightProportional);
        phase = WeightProportional;
    }
}

} //namespace

