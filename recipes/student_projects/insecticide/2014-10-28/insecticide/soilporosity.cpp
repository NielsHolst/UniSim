/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "soilporosity.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (soilporosity)
	
soilporosity::soilporosity(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, fa, 0.25);     //fraction of air in the soil
    Input (double, fw, 0.25);     //fraction of water in the soil

    Output (double, Sp);

}

void soilporosity::reset() {

    Sp = 0.;

}

void soilporosity::update() {

    Sp = fa + fw;

}

} //namespace

