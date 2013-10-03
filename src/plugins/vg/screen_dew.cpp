/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/name.h>
#include "screen_dew.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
ScreenDew::ScreenDew(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(timeStepSecs), "calendar[timeStepSecs]");
    addParameterRef<int>(Name(numTimeSteps), "calendar[totalTimeSteps]");
    addParameterRef<double>(Name(state), "../control[state]");
    addParameterRef<double>(Name(Tscreen), "../temperature[value]");
    addParameterRef<double>(Name(mcIndoors), "climate/humidity[mc]");
    addVariable<double>(Name(value), "Dew formation at all screens (g/m2/s)");
}

void ScreenDew::reset() {
    value = 0.;
}

void ScreenDew::update() {
    const int oneDay = 24*60*60;
    if (timeStepSecs*numTimeSteps < oneDay) {
        value = 0.;
    }
    else {
        const int sides = 2;
        const double gcnd = 3e-3; //  m/s
        double smc = saturatedMoistureContent(Tscreen);
        value = (mcIndoors < smc) ? 0. :
            gcnd*(mcIndoors - smc)*sides*state;
    }
}

} //namespace

