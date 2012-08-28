/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "energy_budget.h"

using namespace UniSim;


namespace SupplyDemand {

EnergyBudget::EnergyBudget(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
}

void EnergyBudget::initialize() {
}

void EnergyBudget::update() {
}

} //namespace

