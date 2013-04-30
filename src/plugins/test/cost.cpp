/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/name.h>
#include "cost.h"

using namespace UniSim;

namespace test{

Cost::Cost(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    addParameterRef<int>(Name(energy), "operation[energy]");
    addParameterRef<int>(Name(labour), "operation[labour]");
    addParameterRef<int>(Name(CO2), "operation[CO2]");

    addParameter<double>(Name(energyRate), 5., "description");
    addParameter<double>(Name(labourRate), 10., "description");
    addParameter<double>(Name(CO2Rate), 15., "description");

    addVariable<double>(Name(energyCost), "description");
    addVariable<double>(Name(labourCost), "description");
    addVariable<double>(Name(CO2Cost), "description");
}

void Cost::reset() {
    energyCost = labourCost = CO2Cost = 0;
}

void Cost::update() {
    energyCost = energy*energyRate;
    labourCost = labour*labourRate;
    CO2Cost = CO2*CO2Rate;
}

} //namespace

