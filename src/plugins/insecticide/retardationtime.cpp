/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "retardationtime.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (retardationtime)
	
retardationtime::retardationtime(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Rf1, 0.); //retardation factors for layer 1 to 10
    Input (double, Rf2, 0.);
    Input (double, Rf3, 0.);
    Input (double, Rf4, 0.);
    Input (double, Rf5, 0.);
    Input (double, Rf6, 0.);
    Input (double, Rf7, 0.);
    Input (double, Rf8, 0.);
    Input (double, Rf9, 0.);
    Input (double, Rf10, 0.);

    Input (double, Irt1, 0.); //ideal retardation time
    Input (double, Irt2, 0.);
    Input (double, Irt3, 0.);
    Input (double, Irt4, 0.);
    Input (double, Irt5, 0.);
    Input (double, Irt6, 0.);
    Input (double, Irt7, 0.);
    Input (double, Irt8, 0.);
    Input (double, Irt9, 0.);
    Input (double, Irt10, 0.);

    Output (double, Rt1);
    Output (double, Rt2);
    Output (double, Rt3);
    Output (double, Rt4);
    Output (double, Rt5);
    Output (double, Rt6);
    Output (double, Rt7);
    Output (double, Rt8);
    Output (double, Rt9);
    Output (double, Rt10);

}

void retardationtime::reset() {

    Rt1 = 0.;
    Rt2 = 0.;
    Rt3 = 0.;
    Rt4 = 0.;
    Rt5 = 0.;
    Rt6 = 0.;
    Rt7 = 0.;
    Rt8 = 0.;
    Rt9 = 0.;
    Rt10 = 0.;

}

void retardationtime::update() {

    Rt1 = Irt1 * Rf1;
    Rt2 = Irt2 * Rf2;
    Rt3 = Irt3 * Rf3;
    Rt4 = Irt4 * Rf4;
    Rt5 = Irt5 * Rf5;
    Rt6 = Irt6 * Rf6;
    Rt7 = Irt7 * Rf7;
    Rt8 = Irt8 * Rf8;
    Rt9 = Irt9 * Rf9;
    Rt10 = Irt10 * Rf10;

}

} //namespace

