/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "small_hive_beetle.h"

using namespace UniSim;


namespace beehive {

SmallHiveBeetle::SmallHiveBeetle(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
}

void SmallHiveBeetle::initialize() {
    stages = seekChildren<Model*>("*");
    numStages = stages.size();
    for (int i = 0; i < numStages; ++i) {
        Model *m = stages[i]->seekOneChild<Model*>("mass");
        masses.append(m);
        Model *n = stages[i]->seekOneChild<Model*>("number");
        numbers.append(n);
    }
    fecundity = stages.last()->seekOneChild<Model*>("fecundity");
}

void SmallHiveBeetle::reset() {
}

void SmallHiveBeetle::update() {
    double massOutflow, numberOutflow;
    massOutflow = fecundity->pullValue<double>("mass");
    numberOutflow = fecundity->pullValue<double>("number");

    for (int i = 0; i < numStages; ++i) {
        masses[i]->pushValue<double>("inflow", massOutflow);
        massOutflow = masses[i]->pullValue<double>("outflow");

        numbers[i]->pushValue<double>("inflow", numberOutflow);
        numberOutflow = numbers[i]->pullValue<double>("outflow");
    }
}

} //namespace

