/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "winddrift.h"
#include <cmath>
#include "publish.h"


using namespace UniSim;

namespace insecticide {

PUBLISH (winddrift)
	
winddrift::winddrift(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Pass, 1.);
    Input (double, fieldarea, 100.);
    Input (double, Wboom, 3.);
    Input (double, P, 0.);
    Input (double, inflow, 3.6);
    Input (double, Ao, 39.);
    Input (double, A1, 0.90);
    Input (double, Bo, 2.28);
    Input (double, B1, 0.147);

    Output (double, Xpass);
    Output (double, fdXpass);
    Output (double, fdpass);
    Output (double, amountdrifted);

}

void winddrift::reset() {
    fdXpass = 0.;
    Xpass = 0.;
    fdpass = 0.;
    amountdrifted = 0.;

}

void winddrift::update() {

    amountdrifted = calcAmountdrifted(Pass);
}

double winddrift::calcAmountdrifted(double n){
    Xpass = ((n - 0.5)* Wboom)+ P;
    fdXpass = Ao*exp(-Xpass/Bo) + A1*exp(-Xpass/B1);
    fdpass = (Wboom/std::sqrt(fieldarea))* fdXpass;
    return fdpass * inflow;

}

} //namespace

