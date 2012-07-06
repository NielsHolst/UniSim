/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "growth.h"

using namespace UniSim;


namespace beehive {

Growth::Growth(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("maxGrowthRate", &maxGrowthRate, 2500., this, "Description");
    new Parameter<double>("resource", &resource, 1000., this, "Grams of pollen");
    new Parameter<double>("attackRate", &attackRate, 1., this, "Per capita");
    new Parameter<double>("egestion", &egestion, 0.15, this, "Per capita");
    new PullVariable<double>("value", &value, this, "Description");
}

void Growth::initialize() {
    Model *parent = seekParent<Model*>("*");
    consumerMass = parent->seekOneChild<Model*>("mass");
    consumerNumber = parent->seekOneChild<Model*>("number");
    duration = consumerMass->parameter<double>("duration");
}

void Growth::reset() {
    value = maxGrowthRate;
}

void Growth::update() {
    if (maxGrowthRate<=0. || duration<=0.)
        throw Exception("maxGrowthrate and duration should both be larger than zero", this);
    double r = log(maxGrowthRate/duration);
    const double dt = 1.;
    double demand = exp(r*dt)*consumerMass->pullVariable<double>("value")/(1.- egestion);
    double supply = GBFuncResp(demand, attackRate*resource*1000); //convert g to mg
    double sdRatio = (demand > 0) ? supply/demand : 0.;
    value = sdRatio*maxGrowthRate;
}

} //namespace

