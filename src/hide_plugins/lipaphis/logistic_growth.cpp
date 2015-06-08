/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "logistic_growth.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(LogisticGrowth)

LogisticGrowth::LogisticGrowth(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, initialDensity, 5.);
    Input(double, growthRate, 0.01);
    Input(double, carryingCapacity, 500.);
    Input(double, timeStep, 1.);
    Output(double, density);
}

void LogisticGrowth::reset() {
    density = initialDensity;
}

void LogisticGrowth::update() {
    double increment = density*growthRate*(carryingCapacity - density)/carryingCapacity*timeStep;
    density += increment;
}

} //namespace

