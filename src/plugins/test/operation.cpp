/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/file_locations.h>
#include <usbase/name.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "operation.h"

using namespace UniSim;

namespace test{

Operation::Operation(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    addParameter<QDate>(Name(date), QDate(2000,1,1), "description");
    addParameter<int>(Name(cost), 0, "description");
    addParameter<bool>(Name(isOrganic), false, "description");

    addVariable<int>(Name(energy), "description");
    addVariable<int>(Name(labour), "description");
    addVariable<int>(Name(CO2), "description");
}

void Operation::reset() {
    energy = 0;
    labour = 100;
    CO2 = 1000;
}

void Operation::update() {
    energy += 1;
    labour += 1;
    CO2 += 1;
}


} //namespace

