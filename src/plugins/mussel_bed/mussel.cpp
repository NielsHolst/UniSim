/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "mussel.h"

using namespace UniSim;

namespace mussel_bed {
	
Mussel::Mussel(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("initialDensity", &initialDensity, 1., this, "desc");
    new Parameter<double>("lossRate", &lossRate, 0., this, "desc");
    new Parameter<double>("growthRate", &growthRate, 0.1, this, "desc");
    new Variable<double>("density", &density, this, "desc");
}

void Mussel::reset() {
    density = initialDensity;
}

void Mussel::update() {
    density += growthRate - lossRate;
}


} //namespace

