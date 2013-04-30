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
    new Parameter<double>("Tcover", &Tcover, 0., this, "Greenhouse cover temperature (oC)");
    new Parameter<double>("Tscreen", &Tscreen, 0., this, "Greenhouse screen temperature (oC)");
    new Parameter<double>("bnLamps", &bnLamps, 0., this, "Longwave radiation produced by lamps (W/m2)");
    new Parameter<double>("rnLamps", &rnLamps, 0., this, "Shortwave radiation produced by lamps (W/m2)");
    new Parameter<double>("spScreen", &spScreen, 0., this, "Setpoint for screen [0;1]");
    new Parameter<double>("Tindoors", &Tindoors, 0., this, "Indoors temperature (oC)");
    new Parameter<double>("incomingRadiation", &incomingRadiation, 0., this, "Total incoming, natural radiation (W/m2)");
    new Parameter<double>("longwaveEmission", &longwaveEmission, 0., this, "Proportion of longwave radiation emitted by canopy [0;1]");
    new Parameter<double>("shortwaveEmission", &shortwaveEmission, 0., this, "Proportion of longwave radiation emitted by canopy [0;1]");
    new Variable<double>("bnAbsorbed", &bnAbsorbed, this, "Shortwave radiation absorbed by (W/m2)");
    new Variable<double>("rnAbsorbed", &rnAbsorbed, this, "Longwave radiation absorbed by (W/m2)");
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

