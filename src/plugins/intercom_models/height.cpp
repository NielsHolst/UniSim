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
    new PullVariable("height", &height, this);
}

void Height::initialize() {
    setParameter("a", &a, 7.5);
    setParameter("b", &b, 0.0085);
    setParameter("c", &c, 77.9);
    setParameter("m", &m, 624.);
    setParameter("minHeight", &minHeight, 0.5);
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
}

} //namespace

