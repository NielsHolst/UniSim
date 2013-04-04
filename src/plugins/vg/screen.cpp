/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen.h"

using namespace UniSim;

namespace vg {
	
Screen::Screen(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("spMaxOpening", &spMaxOpening, 0.95, this,"");
    new Parameter<bool>("spHumidityPassed", &spHumidityPassed, this, false, "Is either humidity set point passed?");
    new Parameter<int>("type", &type, 1, this,
        "Screen type installed:\n"
        "0- No screens\n"
        "1- Energy screen roof\n"
        "2- Energy screen roof and sidewalls\n"
        "3- Shading screen roof\n"
        "4- Energy screen roof + shading screen (double)\n"
        "5- Energy screen roof and sidewalls + roof and sidewall screens");
}

void Screen::initialize() {
    if (type<0 || type>5) {
        QString msg = "Invalid screen type: %1. Must be in 0..5 range";
        throw Exception(msg.arg(type));
    }
}

} //namespace
