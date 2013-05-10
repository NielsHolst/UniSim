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
    addParameterRef<double>(Name(globRad), "outdoors/records[globRad]");
    addParameter<double>(Name(spDaylight), 10., "Global radiation threshold for daylight (W/m2)");
    addParameter<double>(Name(spHumidityDay), 80., "Set point for relative humidity during the day (%)");
    addParameter<double>(Name(spHumidityNight), 90., "Set point for relative humidity during the night (%)");
    addParameter<double>(Name(spDeltaXBasis), 1., "Set point for delta x (g/m3)");
}

void SpHumidityRegular::reset() {
    update();
}

void SpHumidityRegular::update() {
    spRh = (globRad < spDaylight) ? spHumidityNight : spHumidityDay;
    spDeltaX = spDeltaXBasis;
}


} //namespace

