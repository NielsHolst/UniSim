/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "publish.h"
#include "reduction.h"

using namespace UniSim;

namespace xpest {
	
PUBLISH(Reduction)

Reduction::Reduction(Identifier name, QObject *parent)
    : Damage(name, parent)
{
    Input(double, pestSeverity, 1.);    // Severity of pest (%)
    Input(double, maxDamage, 0.35);     // Max. reduction caused
    Input(double, beta, 1.);            // Exponent to scale for leaf area effect
}

void Reduction::amend() {
    children = seekChildren<Reduction*>("*");
}

void Reduction::update() {
    if (children.isEmpty()) {
        damage = pow0(maxDamage*pestSeverity/100., beta);
    }
    else {
        damage = 0;
        for (int i = 0; i < children.size(); ++i) {
            damage += children.at(i)->pullValue<double>("damage");
        }
    }
    if (damage > 1.) damage = 1.;
}

} //namespace

