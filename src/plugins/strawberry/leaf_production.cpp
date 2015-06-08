/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "leaf_production.h"
#include "publish.h"
using namespace UniSim;

namespace strawberry {
	
PUBLISH(LeafProduction)

LeafProduction::LeafProduction(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, productionRate, 0.1); // Number of leaves produced per day
    Output(double, value);
}

void LeafProduction::reset() {
    value = 0.;
}

void LeafProduction::update() {
    value = productionRate;
}


} //namespace

