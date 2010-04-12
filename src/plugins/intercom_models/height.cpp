/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "height.h"

namespace intercom{

Height::Height(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setState("height", &height);
}

void Height::initialize() {
    setParameter("a", &a, 7.5);
    setParameter("b", &b, 0.0085);
    setParameter("c", &c, 77.9);
    setParameter("m", &m, 624.);
    photoThermalTime = seekOneSibling<Model*>("photoThermalTime");
}

void Height::update() {
    double pt = photoThermalTime->state("total");
    height = a + c/(1. + exp(-b*(pt-m)));
    if (height < 0.5)
        height = 0.5;
    height = height/100;
}

} //namespace

