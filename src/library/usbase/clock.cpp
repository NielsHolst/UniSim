/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "clock.h"

namespace UniSim{

Clock* Clock::_clock = 0;

Clock::Clock()
{
}

QString Clock::id() {
    return "Clock";
}

void Clock::doTick(double hour) {
    emit tick(hour);
}


} //namespace

