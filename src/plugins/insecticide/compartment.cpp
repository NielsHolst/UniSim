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
    addParameter<double>(Name(application), 0., "Applied dosage (a.i. per m2)");
    addParameter<double>(Name(halflife), 7., "Half-life of degredation (days)");
    addVariable<double>(Name(concentration), "Current concentration of insecticide (a.i. per m2)");
}

void Compartment::reset() {
    concentration = 0.;
}

void Compartment::update() {
    concentration += application;
    // Add code for degredation...
}

} //namespace

