/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH(crop)
	
crop::crop(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(int, day, 1);            // sowing time
    Input(double, seeds, 1.);      //proportion of seeds

    Output(double, seedsSown);
}

void crop::reset() {
    hoursPassed = 0;
    seedsSown = 0;
}

void crop::update() {
    ++hoursPassed;
    seedsSown = (hoursPassed == 24*day) ? seeds : 0.;
}

} //namespace

