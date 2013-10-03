/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "environment.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
Environment::Environment(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(co2), 350., "CO2 concentration(ppm)");
    addParameter<double>(Name(windDirection), 0., "Wind direction [0;360]");
    addParameterRef<double>(Name(temperature), "./records[Tair]");
    addParameterRef<double>(Name(rh), "./records[RHair]");
    addParameterRef<double>(Name(radiation), "./records[GlobRad]");
    addParameterRef<double>(Name(diffuseRadiation), "./records[DifRad]");
    addParameterRef<double>(Name(windSpeed), "./records[WindSpeed]");
    addParameterRef<double>(Name(skyTemperature), "./records[Tsky]");
    addVariable<double>(Name(directRadiation), "Direct global radiation (W/m2)");
    addVariable<double>(Name(vp), "Water vapour pressure (Pa)");
    addVariable<double>(Name(mc), "Moisture content (g/m3)");
}

void Environment::update() {
    directRadiation = radiation - diffuseRadiation;
    vp = vg::vp(temperature, rh);
    mc = vg::moistureContent(temperature, vp);

}

} //namespace
