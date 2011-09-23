/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "exposure.h"

using namespace UniSim;


namespace SupplyDemand {

Exposure::Exposure(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable<double>("current", &current, this, "Accumulated exposure index");
    new PullVariable<double>("total", &total, this, "Accumulated exposure index");
}

void Exposure::initialize() {
    Model *larva = seekOne<Model*>("larva/lifetable/number");
    larvae = larva->pullVariablePtr<double>("value");

    Model *maize = seekOne<Model*>("maize");
    pollen = maize->pullVariablePtr<double>("pollen");
}

void Exposure::reset() {
    current = total = 0.;
}

void Exposure::update() {
    current = *larvae*(*pollen);
    total += current;
}

} //namespace

