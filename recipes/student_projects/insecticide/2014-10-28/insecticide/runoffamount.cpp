/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "runoffamount.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (runoffamount)
	
runoffamount::runoffamount(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, P, 25.);
    Input (double, fsand, 0.5);

    Output (double, Q);

}

void runoffamount::reset() {

    Q = 0.;

}

void runoffamount::update() {

    double x = -0.0041626*P + 0.0040395*std::pow(P, 2) - 9.0361e-6*std::pow(P, 3);
    double y = -0.011377*P + 0.0026284*std::pow(P, 2) - 5.8564e-6*std::pow(P, 3);
    Q = (fsand < 0.5) ? x : y;  // x is for loamy soils, and y for sandy soils

}

} //namespace

