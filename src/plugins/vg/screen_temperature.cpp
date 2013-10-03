/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/name.h>
#include "screen_temperature.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
ScreenTemperature::ScreenTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(Tindoors), "climate/temperature[value]");
    addParameterRef<double>(Name(Tcover), "greenhouse/cover/temperature[value]");
    addVariable<double>(Name(value), "Screen temperature (oC)");
}

void ScreenTemperature::reset() {
    value = Tcover;
}

void ScreenTemperature::update() {
    const double zero = 0.;
    const double alfa_o = 1.2*zero + 2.8;  // Bot, 1993
    const double alfa_i = 3;
    value =  ((6+alfa_i)*(Tindoors+T0) +
                6*(Tcover+T0) +
                alfa_o*(Tcover+T0)) / (12 + alfa_o + alfa_i) - T0;
}

} //namespace

