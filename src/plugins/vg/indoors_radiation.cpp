/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "greenhouse_construction.h"
#include "indoors_radiation.h"

using namespace UniSim;

namespace vg {
	
IndoorsRadiation::IndoorsRadiation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<int>(Name(glassType), "construction[iGlassType]");
    addParameterRef<double>(Name(sinb), "calendar[sinb]");
    addParameterRef<double>(Name(outdoorsDirectRadiation), "environment[directRadiation]");
    addParameterRef<double>(Name(outdoorsDiffuseRadiation), "environment[diffuseRadiation]");
    addParameter<double>(Name(diffuseTransmission), 0.79, "Transmission of diffuse light through greenhouse construction [0;1]");
    addVariable<double>(Name(direct), "Direct light transmitted through greenhouse construction (W/m2)");
    addVariable<double>(Name(diffuse), "Diffuse light transmitted through greenhouse construction (W/m2)");
}

void IndoorsRadiation::initialize() {
    switch (GreenhouseConstruction::GlassType(glassType)) {
    case GreenhouseConstruction::Single:
        a = 0.844;
        b = 7.39;
        c = -1.66;
        break;
    case GreenhouseConstruction::Double:
        a = 0.818;
        b = 9.77;
        c = -1.73;
        break;
    case GreenhouseConstruction::Hortiplus:
        a = 0.758;
        b = 7.39;
        c = -1.66;
        break;
    default:
        throw Exception(QString("Unknown glass type number: %1").arg(glassType), this);
    }
}

void IndoorsRadiation::reset() {
    direct = diffuse = 0.;
}

void IndoorsRadiation::update() {
    if (sinb == 0.) {
        direct = diffuse = 0.;
    }
    else {
        double angle = asin(sinb)*180./PI;
        double directTransmission = a/(1. + pow(angle/b, c));
        Q_ASSERT(directTransmission>=0 && directTransmission<=1);
        direct = directTransmission*outdoorsDirectRadiation;
        diffuse = diffuseTransmission*outdoorsDiffuseRadiation;
    }
}


} //namespace

