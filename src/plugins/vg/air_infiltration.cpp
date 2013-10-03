/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include <usbase/exception.h>
#include <usbase/utilities.h>
#include "air_infiltration.h"

using namespace UniSim;

namespace vg {
	
AirInfiltration::AirInfiltration(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(timeStepSecs), "calendar[timeStepSecs]");
    addParameterRef<double>(Name(height), "construction[height]");
    addParameterRef<double>(Name(roofRatio), "construction[roofRatio]");
    addParameterRef<double>(Name(sideRatio), "construction[sideRatio]");
    addParameterRef<double>(Name(windspeed), "environment[windspeed]");
    addParameterRef<double>(Name(screensAirTransmission), "screens[airTransmission]");
    addParameter<double>(Name(leakage), 0.5, "Air exchange through leakage (m3 air/m3 greenhouse/h");
    addVariable<double>(Name(value), "Greenhouse air infiltration (m3 air/m2 greenhouse/s)");
}

void AirInfiltration::reset() {
    value = 0.;
}

void AirInfiltration::update() {
    value = height*(leakage/3600)*windspeed/4.;
    // Reduce leakage ventilation in the roof part, but not the side part.
    value *= (screensAirTransmission*roofRatio+sideRatio)/(roofRatio+sideRatio);
}


} //namespace

