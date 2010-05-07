/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "area.h"
#include "community.h"
#include "plant.h"
#include "photosynthesis.h"
#include "photosynthetic_rate.h"

using namespace UniSim;

namespace intercom{

Community::Community(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    photosynthesis = new Photosynthesis(this);
    setState("grossLightAbsorption", &grossLightAbsorption);
    setState("grossAssimilation", &grossAssimilation);
}

void Community::initialize() {
    plants = seekChildren<Plant*>("*");
    areas = seekDescendants<Area*>("*");
}

void Community::reset() {
}

void Community::update() {
    PhotosyntheticRate psRate = photosynthesis->calculate();
    grossLightAbsorption = psRate.absorption();
    grossAssimilation= psRate.assimilation();
}

} //namespace


