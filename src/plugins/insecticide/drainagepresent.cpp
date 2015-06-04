/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "drainagepresent.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (drainagepresent)
	
drainagepresent::drainagepresent(Identifier name, QObject *parent)
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
    Input (double, Dds, 0.6);      //default value, 0.6 m
    Input (double, startLayer1, 0.);             //sm
    Input (double, startLayer4, 0.8);             //m
    Input (double, startLayer9, 1.);             //m


    Output (double, Dp3);                      //Dp1 to Dp10 drainage present at horizon 1-10
    Output (double, Dp2);
    Output (double, Dp4);
    Output (double, Dp1);
    Output (double, Dp5);
    Output (double, Dp6);
    Output (double, Dp7);
    Output (double, Dp8);
    Output (double, Dp9);
    Output (double, Dp10);


}

void drainagepresent::reset() {
    Dp3 = 0.;
    Dp2 = 0.;
    Dp4 = 0.;
    Dp5 = 0.;
    Dp6 = 0.;
    Dp7 = 0.;
    Dp1 = 0.;
    Dp8 = 0.;
    Dp9 = 0.;
    Dp10 = 0.;

}

void drainagepresent::update() {

    Dp1 = (Dds < startLayer2 && Dds > (startLayer1 - 0.01)) ? 1 : 0;
    Dp2 = (Dds < startLayer3 && Dds > (startLayer2 - 0.01)) ? 1 : 0;
    Dp3 = (Dds < startLayer4 && Dds > (startLayer3 - 0.01)) ? 1 : 0;
    Dp4 = (Dds < startLayer5 && Dds > (startLayer4 - 0.01)) ? 1 : 0;
    Dp5 = (Dds < startLayer6 && Dds > (startLayer5 - 0.01)) ? 1 : 0;
    Dp6 = (Dds < startLayer7 && Dds > (startLayer6 - 0.01)) ? 1 : 0;
    Dp7 = (Dds < startLayer8 && Dds > (startLayer7 - 0.01)) ? 1 : 0;
    Dp8 = (Dds < startLayer9 && Dds > (startLayer8 - 0.01)) ? 1 : 0;
    Dp9 = (Dds < startLayer10 && Dds > (startLayer9 - 0.01)) ? 1 : 0;
    Dp10 = (Dds < startLayer11 && Dds > (startLayer10 - 0.01)) ? 1 : 0;

}

} //namespace

