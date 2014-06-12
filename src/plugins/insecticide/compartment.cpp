/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "compartment.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (Compartment)
	
Compartment::Compartment(Identifier name, QObject *parent)
	: Model(name, parent)
{
//    addParameter<double>(Name(application), 0., "Applied dosage (g per ha)");
//    addParameter<double>(Name(proportionToTechnosphere), 0., "Proportion of the applied dosage to technosphere (per cent)");
//    addVariable<double>(Name(concentration), "Current concentration of insecticide (g per ha)");
    Input (double, application, 0.);
    Input (double, proportionToTechnosphere, 0.);
    Output (double, concentration);

}

void Compartment::reset() {
    concentration = 0.;

}

void Compartment::update() {
    concentration = application*proportionToTechnosphere;


}

} //namespace

