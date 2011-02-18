/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "cumulative_emergence.h"

using namespace UniSim;


namespace seed_emergence {

CumulativeEmergence::CumulativeEmergence(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
    new Parameter<double>("a", &a, 10., this, "Gompertz curve @I a parameter");
    new Parameter<double>("b", &b, 0.008, this, "Gompertz curve @I b parameter");
    new PullVariable<double>("accumulated", &accumulated, this, "Current accumulated emergence (%)");
}

void CumulativeEmergence::initialize() {
    hydrotime = seekOneSibling<Model*>("time");
}

void CumulativeEmergence::reset() {
    accumulated = 0.;
}

void CumulativeEmergence::update() {
    double hydrotimeTotal = hydrotime->pullVariable<double>("total");
    accumulated = 100.*exp(-a*exp(-b*hydrotimeTotal));
}

} //namespace

