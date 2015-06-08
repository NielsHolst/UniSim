/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "community.h"
#include "plant.h"
#include "publish.h"

using namespace UniSim;


namespace conductance {

PUBLISH(Community)

Community::Community(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    Output(double, sum_sz); /*
                     "Total crown zone area of the one or two plants present "
                     "(m @Sup 2 ground area owned per m @Sup 2 ground area available) */
}

void Community::initialize() {
    plants = seekChildren<Plant*>("*");
	if (plants.isEmpty())
        throw Exception("Community has no plants", this);
	else if (plants.size() > 2)
        throw Exception("Max. 2 plants are allowed in community", this);
}

void Community::reset() {
    phase = Unlimited;
    smaller = larger = 0;

    updateTotalCrownZoneArea();
    if (sum_sz > 1) {
        QString msg = "Total crown zone area must be < 1 m2 on day 0; sum_sz = " +
                      QString::number(sum_sz);
        throw Exception(msg, this);
    }
}

void Community::update() {
    while (phaseChanged())
        reUpdatePlants();
}

bool Community::phaseChanged() {
    updateTotalCrownZoneArea();
    if (phase == Unlimited)
        return phaseUnlimitedChanged();
    else if (phase == UnderCompression && plants.size() == 2)
        return phaseUnderCompressionChanged();
    return false;
}

void Community::updateTotalCrownZoneArea() {
    sum_sz = 0.;
    for (int i = 0; i < plants.size(); ++i)
        sum_sz += plants[i]->pullValue<double>("total_sz");
}

bool Community::phaseUnlimitedChanged() {
    bool availableAreaExhausted = sum_sz >= 1.;
    if (availableAreaExhausted) {
        sortPlants();
        smaller->changePhase(UnderCompression);
        phase = UnderCompression;
        return true;
    }
    return false;
}

void Community::sortPlants() {
    bool onlyOnePlant = plants.size() == 1;
    if (onlyOnePlant) {
        smaller = plants[0];
        larger = 0;
        return;
    }

    bool firstIsSmaller = plants[0]->pullValue<double>("weight") <
                          plants[1]->pullValue<double>("weight");
    if (firstIsSmaller) {
        smaller = plants[0];
        larger = plants[1];
    }
    else {
        smaller = plants[1];
        larger = plants[0];
    }
}

bool Community::phaseUnderCompressionChanged() {
    bool plantsAreEven = smaller->pullValue<double>("Lz") >=
                         larger->pullValue<double>("Lz");
    if (plantsAreEven)
    {
        smaller->changePhase(WeightProportional);
        larger->changePhase(WeightProportional);
        phase = WeightProportional;
        return true;
    }
    return false;
}

void Community::reUpdatePlants() {
    for (int i = 0; i < plants.size(); ++i)
        plants[i]->reUpdate();
}

} //namespace

