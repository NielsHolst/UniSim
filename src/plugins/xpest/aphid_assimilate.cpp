/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "aphid_assimilate.h"

using std::min;
using namespace UniSim;

namespace xpest {
	
AphidAssimilate::AphidAssimilate(Identifier name, QObject *parent)
    : Damage(name, parent)
{
    addParameter<double>(Name(feedingRate), 0.5, "Feeding rate per aphid mass (g/g/d)");
    addParameter<double>(Name(individualMass), 0.001, "Average mass per aphid (g)");
    addParameter<double>(Name(density), 10., "Aphid density (per m2)");
}

void AphidAssimilate::reset() {
    Damage::reset();
}

void AphidAssimilate::update() {
    damage = feedingRate*individualMass*density;
}

} //namespace

