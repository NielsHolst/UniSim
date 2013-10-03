/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "cover_temperature.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
CoverTemperature::CoverTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(Tindoors), "climate/temperature[value]");
    addParameterRef<double>(Name(Toutdoors), "environment[temperature]");
    addParameterRef<double>(Name(Tsky), "environment[skyTemperature]");
    addParameterRef<double>(Name(windspeed), "environment[windspeed]");
    addParameterRef<double>(Name(latcov), "greenhouse/construction[latcov]");
    addVariable<double>(Name(value), "Cover temperature (oC)");
}

void CoverTemperature::reset() {
    value = Toutdoors;
}

void CoverTemperature::update() {
    double alfa_o = (windspeed < 4) ?
                1.2*windspeed + 2.8 //%Bot, 1993
                : pow(2.5*windspeed, 0.8);
    const double alfa_i = 3;

    value = ((6+alfa_i)*(Tindoors+T0) +
                   6*(Tsky+T0) +
                   alfa_o*(Toutdoors+T0) +
                   latcov) / (12 + alfa_o + alfa_i) - T0;
    //NB. Original returned temperature in K not oC.
}

} //namespace

