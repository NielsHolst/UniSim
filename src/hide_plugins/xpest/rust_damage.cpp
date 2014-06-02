/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "rust_damage.h"

using namespace UniSim;

namespace xpest {
	
RustDamage::RustDamage(Identifier name, QObject *parent)
    : Damage(name, parent)
{
    addParameter<double>(Name(pestSeverity), 1., "Severity of pest (%)");
    addParameter<double>(Name(lai), 0., "Leaf area index");
    addParameter<double>(Name(pustuleArea), 1e-6, "Area on one pustule (m2)");
    addParameter<double>(Name(pustuleDemand), 4.62e-6, "Assimilate demand of one pustule (g/d)");
}

void RustDamage::update() {
    damage = pestSeverity/100. * lai/pustuleArea * pustuleDemand;
}

} //namespace

