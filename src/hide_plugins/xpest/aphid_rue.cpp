/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "aphid_rue.h"

using std::min;
using namespace UniSim;

namespace xpest {
	
AphidRue::AphidRue(Identifier name, QObject *parent)
    : Damage(name, parent)
{
    addParameter<double>(Name(assimilateEaten), 0., "Amount of assimilate eaten (g/m2/d)");
    addParameter<double>(Name(honeyProportion), 0.35, "Proportion of honey produced from assimilates [0;1]");
    addParameter<double>(Name(dewProportion), 0.015, "Rue reduction by honey (m2/g)]");
    addParameter<double>(Name(maxRueReduction), 0.2, "Max. reduction in RUE [0;1]");

    addVariable<double>(Name(honey), "Total amount of honey produced (g/m2)");
}

void AphidRue::reset() {
    Damage::reset();
    honey = 0.;
}

void AphidRue::update() {
    honey += honeyProportion*assimilateEaten;
    damage = min(dewProportion*honey, maxRueReduction);
}

} //namespace

