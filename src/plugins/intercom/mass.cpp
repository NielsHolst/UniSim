/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include "mass.h"

using namespace UniSim;

namespace intercom{

Mass::Mass(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Update, ChildrenLast);
    new Parameter<double>("allocation", &allocation, 0., this,
                             "Allocated dry matter (g per plant per day) to be added to mass");
    new Variable<double>("value", &value, this,
                             "The mass of this organ per plant (g per plant)");
    new Variable<double>("allocationRate", &allocationRate, this,
                             "The mass just allocated to this organ per plant (g per plant per day)");

    new Variable<double>("currentPartition", &currentPartition, this,
                             "The proportion [0;1] that this mass consistutes out of the plant total");
}

void Mass::initialize() {
    mass = seekOneChild<Model*>("mass");
}

void Mass::reset() {
    allocation = 0.;
    value = mass->pullValue<double>("number");
}


void Mass::update() {
    Q_ASSERT(allocation >= 0.);
    mass->pushValue<double>("inflow", allocation);
    allocationRate = allocation;
    allocation = 0.;
    value = mass->pullValue<double>("number");
}

void Mass::updateCurrentPartition(double total) {
    currentPartition = (total == 0) ? 0 : value/total;
}

} //namespace

