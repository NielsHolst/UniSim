/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "WindDrift.h"
#include "publish.h"
#include "iostream"


using namespace UniSim;

namespace insecticide {

PUBLISH (WindDrift)
	
WindDrift::WindDrift(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input (double, Pass, 1.);
    Input (double, Wboom, 3);
    Input (double, P, 0);
    Input (double, inflow, 3.6);
    Input (double, A, 10000);

    Output (double, fdXpass);
    Output (double, fdpass);
    Output (double, amountdrifted);
    Output (double, Xpass);
}

void WindDrift::reset() {
    fdXpass = 0.;
    Xpass = 0.;
    fdpass = 0.;
    amountdrifted = 0.;

}

void WindDrift::update() {
    amountdrifted = calcAmountdrifted(Pass);
}

double WindDrift::calcAmountdrifted(double n) {
    Xpass = (n - 0.5)* Wboom+ P;
    fdXpass = 0.1707 * exp(-0.0958*Xpass);
    fdpass = Wboom/std::sqrt(A)* fdXpass;
    return fdpass * inflow;
}

} //namespace

