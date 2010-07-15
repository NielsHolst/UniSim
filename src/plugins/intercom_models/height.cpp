/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "height.h"

using namespace UniSim;

namespace intercom{

Height::Height(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable("height", &height, this, "description");
}

void Height::initialize() {
    setParameter("a", &a, 7.5, "description");
    setParameter("b", &b, 0.0085, "description");
    setParameter("c", &c, 77.9, "description");
    setParameter("m", &m, 624., "description");
    setParameter("minHeight", &minHeight, 0.5, "description");
    photoThermalTime = seekOneSibling<Model*>("photoThermalTime");
}

void Height::reset() {
    updateHeight(0.);
}

void Height::update() {
    double pt = photoThermalTime->pullVariable("total");
    updateHeight(pt);
}

void Height::updateHeight(double time) {
    height = a + c/(1. + exp(-b*(time - m)));
    if (height < minHeight)
        height = minHeight;
    height = height/100;  // from cm to m
    height = 0.08;
}

} //namespace

