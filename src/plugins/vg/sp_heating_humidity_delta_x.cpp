/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "sp_heating_humidity_delta_x.h"

using namespace UniSim;

namespace vg {
	
SpHeatingHumidityDeltaX::SpHeatingHumidityDeltaX(Identifier name, QObject *parent)
    : SpHeatingHumidityBase(name, parent)
{
    new Parameter<double>("spDeltaX", &spDeltaX, 1, this, "Setpoint for delta x (g/m3)");
    new Parameter<double>("moistureDeficit", &moistureDeficit, 2, this, "Moisture deficit (g/m3)");
}

void SpHeatingHumidityDeltaX::update() {
    double deltaRX = (spDeltaX - moistureDeficit)*1.5;
    spIncrement = (deltaRX > 0) ?
        spMaxIncrement*(1-exp(-(deltaRX*10/pBandRh)/spMaxIncrement)) : 0.;
}


} //namespace

