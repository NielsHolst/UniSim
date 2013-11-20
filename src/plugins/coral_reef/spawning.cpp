/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "spawning.h"

using namespace UniSim;

namespace coral_reef {
	
Spawning::Spawning(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<int>(Name(actualDay), 1, "Current Julian day of year");
    addParameter<int>(Name(spawningDay), 310, "Julian day of year when spawning happens");
    addParameter<double>(Name(fecundity), 1e5, "Number of eggs produced per fish");
    addParameter<double>(Name(noOfFish), 0., "Number of fish spawning");
    addVariable<double>(Name(noOfEggs), "Number of eggs produced in current time step");
}

void Spawning::reset() {
    noOfEggs = 0.;
    prevActualDay = actualDay;
}

void Spawning::update() {
    bool passedSpawningDay = (prevActualDay <= spawningDay) && (spawningDay <= actualDay);
    noOfEggs = passedSpawningDay ? noOfFish*fecundity : 0.;
    prevActualDay = actualDay;
}

} //namespace

