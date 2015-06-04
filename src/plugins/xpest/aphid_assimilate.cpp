/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "aphid_assimilate.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace xpest {

PUBLISH(AphidAssimilate)

AphidAssimilate::AphidAssimilate(Identifier name, QObject *parent)
    : Damage(name, parent)
{
    Input(double, feedingRate, 0.5); // Feeding rate per aphid mass (g/g/d)
    Input(double, individualMass, 0.001); // Average mass per aphid (g)
    Input(double, density, 10.); // Aphid density (per m2)
}

void AphidAssimilate::reset() {
    Damage::reset();
}

void AphidAssimilate::update() {
    damage = feedingRate*individualMass*density;
}

} //namespace

