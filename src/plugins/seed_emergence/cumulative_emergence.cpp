/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include "cumulative_emergence.h"

using namespace UniSim;


namespace seed_emergence {

CumulativeEmergence::CumulativeEmergence(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
    Input(double, a, 10.); // Gompertz curve @I a parameter
    Input(double, b, 0.008); // Gompertz curve @I b parameter
    Output(double, accumulated); // Current accumulated emergence (%)
}

void CumulativeEmergence::initialize() {
    hydrotime = seekOneSibling<Model*>("time");
}

void CumulativeEmergence::reset() {
    accumulated = 0.;
}

void CumulativeEmergence::update() {
    double hydrotimeTotal = hydrotime->pullValue<double>("total");
    accumulated = 100.*exp(-a*exp(-b*hydrotimeTotal));
}

} //namespace

