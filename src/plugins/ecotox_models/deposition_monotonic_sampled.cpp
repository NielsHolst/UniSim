/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "deposition_monotonic_sampled.h"

using namespace UniSim;

namespace ecotox {

DepositionMonotonicSampled::DepositionMonotonicSampled(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable<double>("value", &value, this,
                           "Deposition rate (0..1)");
    new PullVariable<double>("total", &total, this,
                          "Accumulated deposition total (0..1)");
    new PullVariable<QString>("columnName", &columnName, this,
                          "Name of column in @F weather records, from which deposition data is read");
}

void DepositionMonotonicSampled::initialize() {
    Model *weather = seekOne<Model*>("weather");
    airPollen = weather->pullVariablePtr<double>(columnName);
}

void DepositionMonotonicSampled::reset() {
    value = total = 0.;
}

void DepositionMonotonicSampled::update() {
    value = *airPollen;
    total += value;
}

} //namespace

