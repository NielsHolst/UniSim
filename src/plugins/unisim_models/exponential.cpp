/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "exponential.h"

namespace UniSim{
	
Exponential::Exponential(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("initialValue", &initialValue, 0.05, this, "Value at time zero (size or density units)");
    new Parameter<double>("growthRate", &growthRate, 1.0, this, "Exponential growth rate (size or density units per time unit");

    new PullVariable<double>("value", &value, this, "Current value (size or density units)");
    new PullVariable<double>("increment", &increment, this, "Increment in value during the latest time step (size or density units)");
}

void Exponential::initialize() {
    time = seekOneNearest<Model*>("time");
}

void Exponential::reset() {
    value = initialValue;
    increment = 0;
}

void Exponential::update() {
    double t = time->pullVariable<double>("total");
    double valueBefore = value;
    value = initialValue*exp(growthRate*t);
    increment = value - valueBefore;
}

} //namespace

