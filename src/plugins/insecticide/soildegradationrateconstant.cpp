/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "soildegradationrateconstant.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (soildegradationrateconstant)
	
soildegradationrateconstant::soildegradationrateconstant(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, DT50, 1.);     //half-life in the soil (days)

    Output (double, k);           //soil degradation rate constant (h-1)

}

void soildegradationrateconstant::reset() {

    k = 0.;

}

void soildegradationrateconstant::update() {

    k = 0.693/(DT50 * 24.);

}

} //namespace

