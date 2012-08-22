/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "growth_demand.h"
#include "life_stage.h"

using namespace UniSim;

namespace SupplyDemand {

GrowthDemand::GrowthDemand(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("maxGrowthRate", &maxGrowthRate, 30., this, "Maximum finite growth rate through the whole stage (-)");
    new Variable<double>("value", &value, this, "Demand to reach optimal growth this time step (g)");
}

void GrowthDemand::initialize() {
    Model *stage = seekNearestAscendant<LifeStage*>("*");
    Model *massModel = stage->seekOneDescendant<Model*>("lifetable/mass");
    mass = massModel->pullValuePtr<double>("value");
    duration = massModel->pullValuePtr<double>("duration");

    Model *time = seekOneNearest<Model*>("time");
    timeStep = time->pullValuePtr<double>("step");
}

void GrowthDemand::reset() {
    update();
}

void GrowthDemand::update() {
    if (maxGrowthRate < 1)
        throw Exception("The variable 'maxGrowthRate' must be >=1 is " + QString::number(maxGrowthRate), this);
    double instantaneousRate = log(maxGrowthRate/(*duration));
    value = (exp(instantaneousRate*(*timeStep)) - 1.)*(*mass);
}

} //namespace

