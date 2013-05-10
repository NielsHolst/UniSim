/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
//#include <usbase/name.h>
#include "cover.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
Cover::Cover(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(Tindoors), "indoors/temperature[air]");
    addParameterRef<double>(Name(Toutdoors), "outdoors/records[Tair]");
    addParameterRef<double>(Name(Tsky), "sky[temperature]");
    addParameterRef<double>(Name(windspeed), "outdoors/records[windspeed]");
    addParameterRef<double>(Name(latcov), "greenhouse[latcov]");
    addVariable<double>(Name(temperature), "Cover temperature");
}

void Cover::reset() {
    update();
}

void Cover::update() {
    double alfa_o = (windspeed < 4) ?
                1.2*windspeed + 2.8 //%Bot, 1993
                : pow(2.5*windspeed, 0.8);
    const double alfa_i = 3;

    temperature = ((6+alfa_i)*(Tindoors+T0) +
                   6*(Tsky+T0) +
                   alfa_o*(Toutdoors+T0) +
                   latcov) / (12 + alfa_o + alfa_i) - T0;
    //NB. Original returned temperature in K not oC.
}

} //namespace
