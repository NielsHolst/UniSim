/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "lipaphis_demand.h"
#include "publish.h"

using namespace UniSim;

namespace lipaphis {

PUBLISH(LipaphisDemand)

LipaphisDemand::LipaphisDemand(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, demandRate, 0.13);
    Input(double, numPredators, 1);
    Input(double, timeStep, 0);
    Output(double, totalDemand);
}

void LipaphisDemand::reset() {
    update();
}

void LipaphisDemand::update() {
    totalDemand = demandRate*numPredators*timeStep;
}

} //namespace

