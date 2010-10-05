/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "height.h"

using namespace UniSim;

namespace intercom{

Height::Height(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("a", &a, 7.5, this, "description");
    new Parameter<double>("b", &b, 0.0085, this, "description");
    new Parameter<double>("c", &c, 77.9, this, "description");
    new Parameter<double>("m", &m, 624., this, "description");
    new Parameter<double>("minHeight", &minHeight, 0.5, this, "description");
    new PullVariable<double>("height", &height, this, "description");
}

void Height::initialize() {
    photoThermalTime = seekOneSibling<Model*>("photoThermalTime");
}

void Height::reset() {
    updateHeight(0.);
}

void Height::update() {
    double pt = photoThermalTime->pullVariable<double>("total");
    updateHeight(pt);
}

void Height::updateHeight(double time) {
    height = a + c/(1. + exp(-b*(time - m)));
    if (height < minHeight)
        height = minHeight;
    height = height/100;  // from cm to m
}

} //namespace

