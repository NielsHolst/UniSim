/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "lake_environment.h"

using namespace UniSim;

namespace lakeoxygen {

LakeEnvironment::LakeEnvironment(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("avgDepth", &avgDepth, 1., this,
    "Average layer depth calulated from maxdepth and number of layers [m]");

    new Parameter<double>("kd", &kd, 1.75, this,
    "Light Attenuation coefficient [-]");

    new PullVariable<double>("light", &light, this,
    "DLI (Daily Light Integral) computed from PAR, converting from "
    "{@Sym mu}mol/m @Sup 2/s to {@Sym mu}mol/m @Sup 2/d");
}

void LakeEnvironment::initialize() {
    // Connect to the models that generate input
    weather = seekOne<Model*>("weather");
}

void LakeEnvironment::update() {
    // Pull the variables from the models the do calculations*/
    double DLI = weather->pullVariable<double>("obsDLI");

    // Calculate the light at a given depth based on exponential relation
    light = DLI*exp(-kd*avgDepth);
}

} //namespace

