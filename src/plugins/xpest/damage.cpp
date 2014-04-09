/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "damage.h"

using namespace UniSim;

namespace xpest {
	
Damage::Damage(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addVariable<double>(Name(damage), "Damage caused (per day or g/m2/d");
}

void Damage::reset() {
    damage = 0.;
}

} //namespace

