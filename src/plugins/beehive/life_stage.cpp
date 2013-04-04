/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "life_stage.h"

using namespace UniSim;


namespace beehive {

LifeStage::LifeStage(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("rate", &rate, 100., this, "Number of eggs laid per individual per day");
    new Variable<double>("size", &size, this, "Size of an individual (mg)");
}

void LifeStage::initialize() {
    number = seekOneChild<Model*>("number");
    mass = seekOneChild<Model*>("mass");
    growth = peekOneChild<Model*>("growth");
}

void LifeStage::reset() {
    size = 0.;
}

void LifeStage::update() {
    double n = number->pullValue<double>("value");
    double m = mass->pullValue<double>("value");
    size = (n > 1e-12) ? m/n : 0.;
    if (growth) {
        double g = growth->pullValue<double>("value");
        if (g > 0)
            mass->pushValue<double>("growthRate", g);
    }
}

} //namespace

