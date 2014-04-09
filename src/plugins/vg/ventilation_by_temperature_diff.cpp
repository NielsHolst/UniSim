/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "ventilation_by_temperature_diff.h"
#include <usbase/utilities.h>

using namespace std;
using namespace UniSim;

namespace vg {
	
VentilationByTemperatureDiff::VentilationByTemperatureDiff(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(indoorsTemperature), "indoors/temperature[value]");
    addParameterRef<double>(Name(outdoorsTemperature), "outdoors[temperature]");
    addParameterRef<double>(Name(ventilationMax), "../maximum[signal]");
    addVariable<double>(Name(signal), "The desired ventilation opening [0;100]");
}

void VentilationByTemperatureDiff::reset() {
    signal = 0;
}

void VentilationByTemperatureDiff::update() {
    double diff = indoorsTemperature - outdoorsTemperature;
    signal = max(ventilationMax - 1./(0.75*exp(-diff/10)), 0.);
}

} //namespace

