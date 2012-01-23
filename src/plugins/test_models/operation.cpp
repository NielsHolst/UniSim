/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/file_locations.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "operation.h"

using namespace UniSim;

namespace test{

Operation::Operation(Identifier name, QObject *parent)
	: Model(name,parent) 
{ 
    new Parameter<QDate>("Date", &date, QDate(2000,1,1), this, "description");
    new Parameter<int>("Cost", &cost, 0, this, "description");
    new Parameter<bool>("IsOrganic", &isOrganic, false, this, "description");

    new PullVariable<int>("energy", &energy, this, "description");
    new PullVariable<int>("labour", &labour, this, "description");
    new PullVariable<int>("CO2", &CO2, this, "description");
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

