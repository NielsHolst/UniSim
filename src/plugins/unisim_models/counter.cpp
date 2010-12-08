/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "counter.h"

namespace UniSim{

Counter::Counter(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<int>("initValue", &initValue, 0, this, "Initial value of counter");
    new PullVariable<int>("value", &value, this, "Current value of counter");
}

void Counter::reset() {
    value = initValue;
}

void Counter::update() {
    ++value;
}


} //namespace

