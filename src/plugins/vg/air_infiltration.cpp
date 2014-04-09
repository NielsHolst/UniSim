/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/utilities.h>
#include "air_infiltration.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
AirInfiltration::AirInfiltration(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(greenhouseVolume), "construction/geometry[volumeTotal]");
    addParameterRef<double>(Name(windspeed), "outdoors[windspeed]");
    addParameter<double>(Name(leakage), 0.5, "Air exchange through leakage (m3 air/m3 greenhouse/h");
    addParameter<double>(Name(roofRatio), 1., "construction[roofRatio]");
    addParameter<double>(Name(sideRatio), 1., "construction[sideRatio]");
    addParameter<double>(Name(screensAirTransmission), 1., "screens[airTransmission]");
    addVariable<double>(Name(rate), "Greenhouse air infiltration (m3 air/h)");
}

void AirInfiltration::reset() {
    rate = 0.;
}

void AirInfiltration::update() {
    // Reduce leakage ventilation in the roof part, but not the side part.
    double reduction = (screensAirTransmission*roofRatio+sideRatio)/(roofRatio+sideRatio);
    rate = reduction*greenhouseVolume*leakage*windspeed/4;
}


} //namespace

