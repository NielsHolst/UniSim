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
    addParameterRef<double>(Name(directOutdoors), "outdoors[dirRad]");
    addParameterRef<double>(Name(diffuseOutdoors), "outdoors/records[difRad]");
    addParameterRef<int>(Name(screenType), "screen[type]");
    addParameterRef<double>(Name(trScreen), "screen/lightTransmission[transmission]");
    addParameterRef<double>(Name(trGhDif), "greenhouse/transmission[diffuse]");
    addParameterRef<double>(Name(trGhDir), "greenhouse/transmission[direct]");

    addVariable<double>(Name(total), "Total indoors radiation");
    addVariable<double>(Name(direct), "Direct indoors radiation");
    addVariable<double>(Name(diffuse), "Diffuse indoors radiation");
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

