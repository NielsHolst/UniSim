/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "run_iterator_fixed.h"

namespace UniSim{

RunIteratorFixed::RunIteratorFixed(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<int>("numIterations", &numIterations, 10, this, "Number of iterations to count");
    new PullVariable<bool>("value", &value, this, "Is current iteration within the set number of iterations?");
    new PullVariable<int>("iteration", &iteration, this, "Number of current iteration");
    new PullVariable<int>("numIterations", &numIterations, this, "Number of iterations to count");
}

void RunIteratorFixed::initialize() {
    iteration = 1;
    value = true;
}

void RunIteratorFixed::cleanup() {
    ++iteration;
    value = (iteration <= numIterations);
}

} //namespace

