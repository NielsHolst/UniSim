/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "compartment.h"

using namespace UniSim;

namespace insecticide {
	
Compartment::Compartment(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(application), 0., "Applied dosage (g per ha)");
    addParameter<double>(Name(proportionToTechnosphere), 0., "Proportion of the applied dosage to technosphere (per cent)");
    addVariable<double>(Name(concentration), "Current concentration of insecticide (g per ha)");
    //addVariable<double>(Name(outflow), "Current concentration of insecticide (g per ha)");
}

void Compartment::reset() {
    concentration = 0.;
    //outflow = 0.;
}

void Compartment::update() {
    concentration = application*proportionToTechnosphere;
    //outflow = concentration;

}

} //namespace

