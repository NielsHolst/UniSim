/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "random_base.h"

namespace UniSim{

RandomBase::RandomBase(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("minValue", &minValue, 0., this, "Minimum random value");
    new Parameter<double>("maxValue", &maxValue, 0., this, "Maximum random value");
    new PullVariable<double>("value", &value, this, "Random value");
}

void RandomBase::update() {
    bool inBounds = false;
    int i = 0;
    const int IMAX = 30;
    while (!inBounds && i<IMAX) {
        value = drawValue();
        inBounds = value>=minValue && value<=maxValue;
        ++i;
    }
    if (value < minValue)
        value = minValue;
    else if (value > maxValue)
        value = maxValue;
}

} //namespace
