/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "day_or_night_base.h"

using namespace UniSim;

namespace vg {
	
DayOrNightBase::DayOrNightBase(Identifier name, QObject *parent)
    : Model(name, parent){
    addVariable<bool>(Name(isDay), "Is it daytime?");
}

void DayOrNightBase::reset() {
    isDay = false;
}

} //namespace

