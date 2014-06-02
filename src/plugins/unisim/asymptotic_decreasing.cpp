/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "asymptotic_decreasing.h"
#include "publish.h"

namespace UniSim{

PUBLISH(AsymptoticDecreasing)

AsymptoticDecreasing::AsymptoticDecreasing(Identifier name, QObject *parent)
    : Model(name, parent)
{
    addParameter<double>(Name(x), 0., "@F x for which to calculate the value");
    addParameter<double>("min", &yMin, 1., "Asymptotic minimum value attained at large $F {x}");
    addParameter<double>("max", &yMax, 0., "Maximum value attained at @F {x=0}");
    addParameter<double>(Name(slope), 0.5, "Slope of curve in the invert units of @F {x} ");
    addVariable<double>(Name(value), "Value calculated from @F {x}");
}

void AsymptoticDecreasing::reset() {
    value = yMin;
}

void AsymptoticDecreasing::update() {
    value = yMin + (yMax - yMin)*negExp(slope*x);
}

} //namespace

