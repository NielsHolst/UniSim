/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "oxygen_concentration.h"

using namespace UniSim;


namespace lakeoxygen {

OxygenConcentration::OxygenConcentration(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("x1", &x1, 14.652, this, "Constant for Dissolved oxygen concentration calculation");
    new Parameter<double>("x2", &x2, -0.41022, this, "Constant for Dissolved oxygen concentration calculation");
    new Parameter<double>("x3", &x3, 0.0079910, this, "Constant for Dissolved oxygen concentration calculation");
    new Parameter<double>("x4", &x4, -0.000077774, this, "Constant for Dissolved oxygen concentration calculation");
    new PullVariable<double>("DOSatConc", &doSatConc, this, "Calculated oxygen concentration estimated from temperature [mg/l]");
}

void OxygenConcentration::initialize(){
    // Connects to the models that generate input
    weather = seekOne<Model*>("weather");
}

void OxygenConcentration::update() {
    // Pull the variables from the models to do calculations
    double temp = weather->pullVariable<double>("tempw");

    // Calculate the Dissolved Oxygen Concentration [g/m3]
    doSatConc = x1 + x2*temp + x3*temp*temp + x4*temp*temp*temp;
}

} //namespace

