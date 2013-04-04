/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "sp_humidity_regular.h"

using namespace UniSim;

namespace vg {
	
SpHumidityRegular::SpHumidityRegular(Identifier name, QObject *parent)
    : SpHumidityBase(name, parent)
{
    new Parameter<double>("globRad", &globRad, 0., this, "Global radiation (W/m2)");
    new Parameter<double>("spDaylight", &spDaylight, 10., this, "Global radiation threshold for daylight (W/m2)");
    new Parameter<double>("spHumidityDay", &spHumidityDay, 80., this, "Set point for relative humidity during the day (%)");
    new Parameter<double>("spHumidityNight", &spHumidityNight, 90., this, "Set point for relative humidity during the night (%)");
    new Parameter<double>("spDeltaXBasis", &spDeltaXBasis, 1., this, "Set point for delta x (g/m3)");
}

void SpHumidityRegular::reset() {
    update();
}

void SpHumidityRegular::update() {
    spRh = (globRad < spDaylight) ? spHumidityNight : spHumidityDay;
    spDeltaX = spDeltaXBasis;
}


} //namespace

