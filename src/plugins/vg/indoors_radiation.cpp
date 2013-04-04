/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "indoors_radiation.h"

using namespace UniSim;

namespace vg {
	
IndoorsRadiation::IndoorsRadiation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("directOutdoors", &directOutdoors, 10., this, "Outdoors direct radiation");
    new Parameter<double>("diffuseOutdoors", &diffuseOutdoors, 10., this, "Outdoors diffuse radiation");
    new Parameter<int>("screenType", &screenType, 1, this, "Screen type installed");
    new Parameter<double>("trScreen", &trScreen, 1, this, "Transmission of light through screen [0;1]");
    new Parameter<double>("trGhDif", &trGhDif, 0.72, this, "Transmission of diffuse light through greenhouse construction [0;1]");
    new Parameter<double>("trGhDir", &trGhDir, 0.5, this, "Transmission of direct light through greenhouse construction [0;1]");

    new Variable<double>("total", &total, this, "Total indoors radiation");
    new Variable<double>("direct", &direct, this, "Direct indoors radiation");
    new Variable<double>("diffuse", &diffuse, this, "Diffuse indoors radiation");
}

void IndoorsRadiation::reset() {
    total = direct = diffuse = 0;
}

void IndoorsRadiation::update() {
    direct = trGhDif*directOutdoors;
    diffuse = trGhDif*diffuseOutdoors;

    direct *= trScreen;
    diffuse *= trScreen;

    if (screenType==1 || screenType==3) {
         // Roof and side wall screens
         direct *= 0.975;
         diffuse *= 0.975;
    }
    else if (screenType > 0){
        // Only roof screen
        direct *= 0.95;
        diffuse *= 0.95;
    }
    total = direct + diffuse;
}


} //namespace

