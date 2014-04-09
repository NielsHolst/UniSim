/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "ventilation_controller.h"
#include <usbase/utilities.h>

using namespace std;
using namespace UniSim;

namespace vg {
	
VentilationController::VentilationController(Identifier name, QObject *parent)
	: Model(name, parent)
{

    addParameter<double>(Name(windSideProportion), 0.75, "The wind side signal as a proportion of the lee side signal [0;1]");
    addParameterRef<double>(Name(indoorsTemperature), "indoors/temperature[value]");
    addParameterRef<double>(Name(outdoorsTemperature), "outdoors[temperature]");
    addParameterRef<double>(Name(Tventilation), "setpoints/temperature/ventilation[setpoint]");
    addParameterRef<double>(Name(windSpeed), "outdoors[windspeed]");
    addParameterRef<double>(Name(byTemperature), "./byTemperature[signal]");
    addParameterRef<double>(Name(byTemperatureDiff), "./byTemperatureDiff[signal]");
    addParameterRef<double>(Name(byHumidity), "./byHumidity[signal]");
    addParameterRef<double>(Name(maxSignal), "./maximum[signal]");
    addVariable<double>("signal", &leeSideSignal, "Synonym for @F {leeSideSignal}");
    addVariable<double>(Name(leeSideSignal), "Ventilation opening on the lee side [0;100]");
    addVariable<double>(Name(windSideSignal), "Ventilation opening on the wind side [0;100]");
}

void VentilationController::reset() {
    leeSideSignal = windSideSignal = 0;
}

void VentilationController::update() {
    bool tooColdIndoors = (indoorsTemperature + 15 < Tventilation);
    bool tooColdOutdoors = outdoorsTemperature < 4;
    leeSideSignal = (tooColdIndoors || tooColdOutdoors) ? 0. :
                    max(min(byHumidity,byTemperatureDiff), byTemperature*windFactor());
    leeSideSignal = min(leeSideSignal, maxSignal);
    windSideSignal = windSideProportion*leeSideSignal;
}

double VentilationController::windFactor() {
    return min(max(windSpeed,1.)/2., 1.);
}


} //namespace

