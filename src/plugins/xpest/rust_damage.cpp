/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "publish.h"
#include "rust_damage.h"

using namespace UniSim;

namespace xpest {
	
PUBLISH(RustDamage)

RustDamage::RustDamage(Identifier name, QObject *parent)
    : Damage(name, parent)
{
    Input(double, pestSeverity, 1.);        // Severity of pest (%)
    Input(double, lai, 0.);                 // Leaf area index
    Input(double, pustuleArea, 1e-6);       // Area on one pustule (m2)
    Input(double, pustuleDemand, 4.62e-6);  // Assimilate demand of one pustule (g/d)
}

void RustDamage::update() {
    damage = pestSeverity/100. * lai/pustuleArea * pustuleDemand;
}

} //namespace

