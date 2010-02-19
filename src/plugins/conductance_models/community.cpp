/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/utilities.h>
#include "community.h"
#include "plant.h"

using namespace UniSim;


namespace conductance {

Community::Community(UniSim::Identifier name, QObject *parent)
    : Model(name, parent), phase(Unlimited) {
}

void Community::initialize() {
    plants = UniSim::findChildren<Plant*>("*", this);
	if (plants.isEmpty())
		throw Exception("Community has no plants");
	else if (plants.size() > 2)
        throw Exception("Max. 2 plants are allowed in community");
}

void Community::reset() {
    smallest = -1;
}

void Community::update() {
    if (phase == Unlimited)
        updateUnlimited();
    else if (phase == UnderCompression)
        updateUnderCompression();
}

void Community::updateUnlimited() {
    double sum_sz = 0.;
    for (int i = 0; i < plants.size(); ++i)
        sum_sz += plants[i]->state("total_sz");

    if (sum_sz >= 1.) {
        smallest = (plants.size() == 1
                   || plants[1]->state("weight") > plants[0]->state("weight")) ? 0: 1;
        plants[smallest]->changePhase(UnderCompression);
        phase = UnderCompression;
    }
}

void Community::updateUnderCompression() {
    Q_ASSERT(smallest>=0 && smallest<=1);
    if (plants[smallest]->state("Lz") >= plants[1-smallest]->state("Lz")) {
        plants[0]->changePhase(WeightProportional);
        plants[1]->changePhase(WeightProportional);
        phase = WeightProportional;
    }
}

} //namespace

