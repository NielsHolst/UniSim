/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "horizonthickness.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (horizonthickness)
	
horizonthickness::horizonthickness(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, startLayer6, 1.);            //m
    Input (double, startLayer2, 0.33);            //m
    Input (double, startLayer3, 0.55);            //m
    Input (double, startLayer10, 1.);           //m
    Input (double, startLayer11, 1.);           //m
    Input (double, startLayer8, 1.);            //m
    Input (double, startLayer7, 1.);            //m
    Input (double, startLayer5, 1.);            //m
    Input (double, penetrationDepth, 0.01);      //default value, 0.01 m
    Input (double, startLayer1, 0.);             //sm
    Input (double, startLayer4, 0.8);             //m
    Input (double, startLayer9, 1.);             //m


    Output (double, Htc3);                      //Htc1 to Htc10 have units of "m"
    Output (double, Htc2);
    Output (double, Htc4);
    Output (double, Htc1);
    Output (double, Htc5);
    Output (double, Htc6);
    Output (double, Htc7);
    Output (double, Htc8);
    Output (double, Htc9);
    Output (double, Htc10);


}

void horizonthickness::reset() {
    Htc3 = 0.;
    Htc2 = 0.;
    Htc4 = 0.;
    Htc5 = 0.;
    Htc6 = 0.;
    Htc7 = 0.;
    Htc1 = 0.;
    Htc8 = 0.;
    Htc9 = 0.;
    Htc10 = 0.;

}

void horizonthickness::update() {

    Htc1 = startLayer2 - startLayer1 - penetrationDepth;
    Htc2 = startLayer3 - startLayer2;
    Htc3 = startLayer4 - startLayer3;
    Htc4 = startLayer5 - startLayer4;
    Htc5 = startLayer6 - startLayer5;
    Htc6 = startLayer7 - startLayer6;
    Htc7 = startLayer8 - startLayer7;
    Htc8 = startLayer9 - startLayer8;
    Htc9 = startLayer10 - startLayer9;
    Htc10 = startLayer11 - startLayer10;

}

} //namespace

