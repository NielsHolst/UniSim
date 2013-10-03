/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screens.h"

using namespace UniSim;

namespace vg {
	
Screens::Screens(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addVariable<double>(Name(temperature), "Average temperature of screens [oC]");
    addVariable<double>(Name(airTransmission), "Transmission of air through all screens [0;1]");
    addVariable<double>(Name(lightTransmission), "Transmission of light through all screens [0;1]");
    addVariable<double>(Name(dew), "Dew formation at all screens (kg/m2/s)");
}

void Screens::initialize() {
    at.set(this, "screens/*/temperature[value]");
    at.set(this, "screens/*/airTransmission[value]");
    lt.set(this, "screens/*/lightTransmission[value]");
    d.set(this, "screens/*/dew[value]");
}

void Screens::reset() {
    temperature = airTransmission = lightTransmission = 1.;
    dew = 0.;
}

void Screens::update() {
    temperature = t.average();
    airTransmission = at.product();
    lightTransmission = lt.product();
    dew = d.sum();
}

} //namespace

