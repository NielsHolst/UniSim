/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "aphid_rue.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace xpest {
	
PUBLISH(AphidRue)

AphidRue::AphidRue(Identifier name, QObject *parent)
    : Damage(name, parent)
{
    Input(double, assimilateEaten, 0.); // Amount of assimilate eaten (g/m2/d)
    Input(double, honeyProportion, 0.35); // Proportion of honey produced from assimilates [0;1]
    Input(double, dewProportion, 0.015);  // Rue reduction by honey (m2/g)]
    Input(double, maxRueReduction, 0.2); // Max. reduction in RUE [0;1]

    Output(double, honey); // Total amount of honey produced (g/m2)
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

