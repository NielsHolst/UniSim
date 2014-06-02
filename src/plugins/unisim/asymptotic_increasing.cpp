/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "asymptotic_increasing.h"
#include "publish.h"

namespace UniSim{

PUBLISH(AsymptoticIncreasing)

AsymptoticIncreasing::AsymptoticIncreasing(Identifier name, QObject *parent)
    : Model(name, parent)
{
    addParameter<double>(Name(x), 0., "@F x for which to calculate the value");
    addParameter<double>("min", &yMin, 0., "Minimum value attained at @F {x=0}");
    addParameter<double>("max", &yMax, 1., "Asymptotic maximum value attained at large $F {x}");
    addParameter<double>(Name(slope), 0.5, "Slope of curve in the invert units of @F {x} ");
    addVariable<double>(Name(value), "Value calculated from @F {x}");
}

void AsymptoticIncreasing::reset() {
    value = yMin;
}

void AsymptoticIncreasing::update() {
    value = yMin + (yMax - yMin)*(1. - negExp(slope*x));
}

} //namespace

