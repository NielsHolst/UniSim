/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "weeds.h"

using namespace UniSim;

namespace xpest {

PUBLISH(Weeds)

Weeds::Weeds(Identifier name, QObject *parent)
    : Damage(name, parent)
{
    Input(double, density, 10.);        // Weed density (per m2)
    Input(double, exponent, 0.003);     // Parameter for the negative exponential function
}

void Weeds::update() {
    damage = 1. - exp(-exponent*density);
}

} //namespace

