/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "grass_productivity.h"

using namespace UniSim;


namespace AnimalStocking {

GrassProductivity::GrassProductivity(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("a", &a, 4.4, this, "Description");
    new Parameter<double>("b", &b, 0.82, this, "Description");
    new PullVariable<double>("dryMatter", &dryMatter, this, "Description");
    new PullVariable<double>("energy", &energy, this, "Description");
}

void GrassProductivity::initialize() {
    environment = seekOne<Model*>("environment");
}

void GrassProductivity::reset() {
    update();
}

void GrassProductivity::update() {
    double dig = environment->pullVariable<double>("digestibility");
    dryMatter = calculateDryMatter();
    energy = dryMatter*a*b*dig;
}

} //namespace

