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
    addParameterRef<double>(Name(outdoorsDirectRadiation), "outdoors[directRadiation]");
    addParameterRef<double>(Name(outdoorsDiffuseRadiation), "outdoors[diffuseRadiation]");
    addParameterRef<double>(Name(diffuseTransmission), "indoors/surface[diffuseTransmission]");
    addParameterRef<double>(Name(directTransmissionAsDirect), "indoors/surface[directTransmissionAsDirect]");
    addParameterRef<double>(Name(directTransmissionAsDiffuse), "indoors/surface[directTransmissionAsDiffuse]");

    addVariable<double>(Name(direct), "Direct light transmitted through greenhouse construction (W/m2)");
    addVariable<double>(Name(diffuse), "Diffuse light transmitted through greenhouse construction (W/m2)");
    addVariable<double>(Name(total), "Total light transmitted through greenhouse construction (W/m2)");
}

void IndoorsRadiation::reset() {
    direct = diffuse = total = 0.;
}

void IndoorsRadiation::update() {
    diffuse = diffuseTransmission*outdoorsDiffuseRadiation + directTransmissionAsDiffuse*outdoorsDirectRadiation;
    direct = directTransmissionAsDirect*outdoorsDirectRadiation;
    total = direct + diffuse;
}


} //namespace

