/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_radiation_absorption.h"

using namespace UniSim;

namespace vg {
	
IndoorsRadiationAbsorption::IndoorsRadiationAbsorption(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(Tcover), "greenhouse/cover[temperature]");
    addParameterRef<double>(Name(Tscreen), "greenhouse/screen/temperature[temperature]");
    addParameterRef<double>(Name(bnLamps), "greenhouse/lamps/yield[bnLight]");
    addParameterRef<double>(Name(rnLamps), "greenhouse/lamps/yield[rnLight]");
    addParameterRef<double>(Name(spScreen), "screen/lightTransmission[transmission]"); // combine screens!
    addParameterRef<double>(Name(Tindoors), "indoors/temperature[air]");
    addParameterRef<double>(Name(incomingRadiation), "indoors/radiation[total]");
    addParameter<double>(Name(longwaveEmission), 0., "Proportion of longwave radiation emitted by canopy [0;1]");
    addParameter<double>(Name(shortwaveEmission), 0., "Proportion of longwave radiation emitted by canopy [0;1]");
    addVariable<double>(Name(bnAbsorbed), "Shortwave radiation absorbed by (W/m2)");
    addVariable<double>(Name(rnAbsorbed), "Longwave radiation absorbed by (W/m2)");
}

void IndoorsRadiationAbsorption::reset() {
    bnAbsorbed = rnAbsorbed = 0.;
}

void IndoorsRadiationAbsorption::update() {
    double Tshelter = Tscreen*spScreen + Tcover*(1. - spScreen);
    bnAbsorbed = shortwaveEmission*Sigma*(pow(Tindoors+T0, 4.) - pow(Tshelter+T0, 4.)) + bnLamps;
    rnAbsorbed = longwaveEmission*(incomingRadiation + rnLamps);
}


} //namespace

