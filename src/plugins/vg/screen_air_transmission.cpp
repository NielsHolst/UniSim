/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_air_transmission.h"

using namespace UniSim;

namespace vg {
	
ScreenAirTransmission::ScreenAirTransmission(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(spScreenEnergy), "screen/energy[sp]");
    addParameterRef<double>(Name(spScreenShade), "screen/shade[sp]");
    addParameterRef<double>(Name(spScreenBlackout), "screen/blackout[sp]");
    addParameter<double>(Name(trScreenEnergy), 0.29, "Air transmission of energy screen [0;1]");
    addParameter<double>(Name(trScreenShade), 0.6, "Air transmission of shade screen [0;1]");
    addParameter<double>(Name(trScreenBlackout), 0, "Air transmission of blackout screen [0;1]");

    addVariable<double>(Name(transmission), "Transmission of air through screen [0;1]");
}

void ScreenAirTransmission::reset() {
    transmission = 0.;
}

void ScreenAirTransmission::update() {
    // From radi.m
    double a = 1 - (1 - trScreenShade)*spScreenShade;
    double b = 1 - (1 - trScreenEnergy)*spScreenEnergy;
    double c = 1 - (1 - trScreenBlackout)*spScreenBlackout;
    transmission = a*b*c;
}

} //namespace

