/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "day_or_night_by_sun.h"

using namespace UniSim;

namespace vg {
	
DayOrNightBySun::DayOrNightBySun(Identifier name, QObject *parent)
    : DayOrNightBase(name, parent)
{
    addParameterRef<double>(Name(sinb), "calendar[sinb]");
}

void DayOrNightBySun::update() {
    isDay = sinb > 0;
}

} //namespace

