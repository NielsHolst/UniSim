/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_temperature.h"
#include "general.h"

using namespace UniSim;

namespace vg {
	
ScreenTemperature::ScreenTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("Tindoors", &Tindoors, 27., this, "Indoors temperature");
    new Parameter<double>("Tcover", &Tcover, 10., this, "Cover temperature");
    new Variable<double>("temperature", &temperature, this, "Screen temperature (oC)");
}

void ScreenTemperature::reset() {
    update();
}

void ScreenTemperature::update() {
    const double windSpeed = 0.;
    double alfa_o = 1.2*windSpeed + 2.8; // Bot, 1993
    double alfa_i = 3;
    temperature = ((6+alfa_i)*(Tindoors+T0) + (6+alfa_o)*(Tcover+T0)) / (12 + alfa_o + alfa_i) - T0;
    //NB. Original returned temperature in K not oC.
}


} //namespace

