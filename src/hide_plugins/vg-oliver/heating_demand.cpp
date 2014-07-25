/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "heating_demand.h"
#include "pipe.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
HeatingDemand::HeatingDemand(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(Tminimum), "setpoints/temperature[minimum]");
    addParameterRef<double>(Name(energyBalance), "greenhouse/energetics[energyBalance]");
    addParameterRef<double>(Name(heatCapacity), "greenhouse/energetics[heatCapacity]");

    addVariable<double>(Name(total), "Total heat needed to reach desired minimum temperature (W/m2)");
    addVariable<double>(Name(perPipe), "@F total divided by number of pipes (W/m2)");
}

void HeatingDemand::initialize() {
    Model *parent = seekParent<Model*>("*");
    QList<Pipe*> pipes = parent->seekDescendants<Pipe*>("*");
    numPipes = pipes.size();
}

void HeatingDemand::reset() {
    total = perPipe = 0.;
}

void HeatingDemand::update() {
    total = max(heatCapacity*(Tminimum + T0) - energyBalance, 0.);
    perPipe = total/numPipes;
}

} //namespace

