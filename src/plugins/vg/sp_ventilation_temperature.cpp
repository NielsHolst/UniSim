/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "sp_ventilation_temperature.h"

using namespace UniSim;

namespace vg {
	
SpVentilationTemperature::SpVentilationTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("spHeating", &spHeating, 24., this, "Set point for heating (oC)");
    new Parameter<double>("heatingIncrement", &heatingIncrement, 1., this, "The basic ventilation set point is incremented this much over the heating set point (oC)");
    new Parameter<double>("humidityDecrement", &humidityDecrement, 1., this, "The basic ventilation set point is incremented this much over the heating set point (oC)");
    new Variable<double>("sp", &sp, this, "Set point for ventilation (oC)");
}

void SpVentilationTemperature::update() {
    sp = spHeating + heatingIncrement - humidityDecrement;
}


} //namespace

