/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_light_transmission.h"

using namespace UniSim;

namespace vg {
	
ScreenLightTransmission::ScreenLightTransmission(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("trScreenEnergy", &trScreenEnergy, 0.3, this, "Light transmission of energy screen [0;1]");
    new Parameter<double>("trScreenShade", &trScreenShade, 0.4, this, "Light transmission of shade screen [0;1]");
    new Parameter<double>("trScreenBlackout", &trScreenBlackout, 0, this, "Light transmission of blackout screen [0;1]");
    new Parameter<double>("spScreenEnergy", &spScreenEnergy, 0, this, "Set point for energy screen [0;1]");
    new Parameter<double>("spScreenShade", &spScreenShade, 0, this, "Set point for shade screen [0;1]");
    new Parameter<double>("spScreenBlackout", &spScreenBlackout, 0, this, "Set point for blackout screen [0;1]");

    new Variable<double>("transmission", &transmission, this, "Transmission of light through screen [0;1]");
}

void ScreenLightTransmission::reset() {
    update();
}

void ScreenLightTransmission::update() {
    // From radi.m
    double a = 1 - (1 - trScreenShade)*spScreenShade;
    double b = 1 - (1 - trScreenEnergy)*spScreenEnergy;
    double c = 1 - (1 - trScreenBlackout)*spScreenBlackout;
    transmission = a*b*c;
}

} //namespace

