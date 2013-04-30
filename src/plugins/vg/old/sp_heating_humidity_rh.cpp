/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "sp_heating_humidity_rh.h"

using namespace UniSim;

namespace vg {
	
SpHeatingHumidityRh::SpHeatingHumidityRh(Identifier name, QObject *parent)
    : SpHeatingHumidityBase(name, parent)
{
    new Parameter<double>("pBand", &pBand, 10., this, "Slope for max heating increment due to humidity (oC)");
    new Parameter<double>("spRh", &spRh, 85, this, "Setpoint for relative humidity (%)");
    new Parameter<double>("rh", &rh, 80, this, "Indoors humidity (%)");
}

void SpHeatingHumidityRh::update() {
    double deltaRh = (rh - spRh)*1.5;
    spIncrement = propControl(deltaRh, pBand, spMaxIncrement);
}


} //namespace

