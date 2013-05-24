/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_shade.h"

using namespace UniSim;

namespace vg {
	
ScreenShade::ScreenShade(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(globRad), "outdoors/records[globRad]");
    addParameterRef<double>(Name(Tindoors), "indoors/temperature[air]");
    addParameter<double>(Name(spLight), 10., "Global radiation threshold for using shade screen (W/m2)");
    addParameterRef<double>(Name(spTemperature), "ventilation/spOpening/alphaTemperature/spTemperature[sp]");
    addParameterRef<double>(Name(spScreenEnergy), "screen/energy[sp]");
    addVariable<double>(Name(sp),"Set point for energy screen [0;1]");
}

void ScreenShade::initialize() {
    Model *parent = seekParent<Model*>("*");
    type = parent->pullValuePtr<int>("type");
    spMaxOpening = parent->pullValuePtr<double>("spMaxOpening");
    spHumidityPassed = parent->pullValuePtr<bool>("spHumidityPassed");
}

void ScreenShade::reset() {
    update();
}

void ScreenShade::update() {
    // Temperature control
    double dT = Tindoors - spTemperature;
    sp = (*type>= 3 && *type<= 5 && globRad>spLight && dT>0) ?
        1 - exp(-dT*globRad/2000) :
        0.;
    // Humidity control
    if (*spHumidityPassed)
        sp *= *spMaxOpening;
    // Interaction with energy screen
    if (spScreenEnergy > sp)
        sp = spScreenEnergy;
}

} //namespace
