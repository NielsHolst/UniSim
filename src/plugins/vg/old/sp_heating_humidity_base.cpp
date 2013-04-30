/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "sp_heating_humidity_base.h"

using namespace UniSim;

namespace vg {
	
SpHeatingHumidityBase::SpHeatingHumidityBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("spMaxIncrement", &spMaxIncrement, 2., this, "Set point for max heating increment due to humidity (oC)");
    new Variable<double>("spIncrement", &spIncrement, this, "Increment in set point for heating (oC)");
}

void SpHeatingHumidityBase::reset() {
    update();
}


} //namespace

