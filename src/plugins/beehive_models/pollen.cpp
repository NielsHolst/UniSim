/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "pollen.h"

using namespace UniSim;


namespace beehive{

Pollen::Pollen(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("initMass", &initMass, 100., this, "Initial mass");
    new Parameter<double>("maxMass", &maxMass, 2000, this, "Max. mass (carrying capacity)");
    new Parameter<double>("r", &r, 0.1, this, "Relative growth rate (per day)");
    new PullVariable<double>("mass", &mass, this, "Current mass");
    new PushVariable<double>("loss", &loss, this, "Loss of mass");
}

void Pollen::reset() {
    mass = initMass;
    loss = 0.;
}

void Pollen::update() {
    mass += (maxMass<=0) ? 0 : mass*r*(maxMass-mass)/maxMass;
    mass -= loss;
    loss = 0;
}

} //namespace

