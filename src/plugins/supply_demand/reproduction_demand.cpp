/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include "reproduction_demand.h"
#include "life_stage.h"

using namespace UniSim;


namespace SupplyDemand {

ReproductionDemand::ReproductionDemand(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Update,ChildrenNot);
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this, "Fixed sex ratio");
    new Parameter<double>("eggWeight", &eggWeight, 0.01, this, "Weight of one egg");
    new Variable<double>("value", &value, this, "Demand for reproduction during this time step (g)");
}

void ReproductionDemand::initialize() {
    unlaidEggs = seekOneChild<Model*>("unlaidEggs");
    laidEggs = unlaidEggs->pullValuePtr<double>("outflow");

    Model *stage = seekNearestAscendant<LifeStage*>("*");
    Model *numberModel = stage->seekOneDescendant<Model*>("lifetable/number");
    numAdults = numberModel->pullValuePtr<double>("value");
    newAdults = numberModel->pullValuePtr<double>("inflow");
}

void ReproductionDemand::reset() {
    value = 0;
}

} //namespace

