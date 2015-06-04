/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "fractiondrainagedepth.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (fractiondrainagedepth)
	
fractiondrainagedepth::fractiondrainagedepth(Identifier name, QObject *parent)
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

    Input (double, Dp3, 0.);                      //drainage present
    Input (double, Dp2, 0.);
    Input (double, Dp4, 0.);
    Input (double, Dp1, 0.);
    Input (double, Dp5, 0.);
    Input (double, Dp6, 0.);
    Input (double, Dp7, 0.);
    Input (double, Dp8, 0.);
    Input (double, Dp9, 0.);
    Input (double, Dp10, 0.);

    Input (double, Htc3, 0.);                      //Htc1 to Htc10 have units of "m"
    Input (double, Htc2, 0.);
    Input (double, Htc4, 0.);
    Input (double, Htc1, 0.);
    Input (double, Htc5, 0.);
    Input (double, Htc6, 0.);
    Input (double, Htc7, 0.);
    Input (double, Htc8, 0.);
    Input (double, Htc9, 0.);
    Input (double, Htc10, 0.);


    Output (double, Fdd3);                      //fraction drainage depth
    Output (double, Fdd2);
    Output (double, Fdd4);
    Output (double, Fdd1);
    Output (double, Fdd5);
    Output (double, Fdd6);
    Output (double, Fdd7);
    Output (double, Fdd8);
    Output (double, Fdd9);
    Output (double, Fdd10);

}

void fractiondrainagedepth::reset() {
    Fdd3 = 0.;
    Fdd2 = 0.;
    Fdd4 = 0.;
    Fdd5 = 0.;
    Fdd6 = 0.;
    Fdd7 = 0.;
    Fdd1 = 0.;
    Fdd8 = 0.;
    Fdd9 = 0.;
    Fdd10 = 0.;

}

void fractiondrainagedepth::update() {

//    Fdd1 = (Htc1 > 0) ? Dp1 * ((startLayer2 - depthOfDrainageSystem)/Htc1) : 0;
//    Fdd2 = (Htc2 > 0) ? Dp2 * ((startLayer3 - depthOfDrainageSystem)/Htc2) : 0;
//    Fdd3 = (Htc3 > 0) ? Dp3 * ((startLayer4 - depthOfDrainageSystem)/Htc3) : 0;
//    Fdd4 = (Htc4 > 0) ? Dp4 * ((startLayer5 - depthOfDrainageSystem)/Htc4) : 0;
//    Fdd5 = (Htc5 > 0) ? Dp5 * ((startLayer6 - depthOfDrainageSystem)/Htc5) : 0;
//    Fdd6 = (Htc6 > 0) ? Dp6 * ((startLayer7 - depthOfDrainageSystem)/Htc6) : 0;
//    Fdd7 = (Htc7 > 0) ? Dp7 * ((startLayer8 - depthOfDrainageSystem)/Htc7) : 0;
//    Fdd8 = (Htc8 > 0) ? Dp8 * ((startLayer9 - depthOfDrainageSystem)/Htc8) : 0;
//    Fdd9 = (Htc9 > 0) ? Dp9 * ((startLayer10 - depthOfDrainageSystem)/Htc9) : 0;
//    Fdd10 = (Htc10 > 0) ? Dp10 * ((startLayer11 - depthOfDrainageSystem)/Htc10) : 0;

    Fdd1 = (Htc1 > 0 && (startLayer2 - Dds)/Htc1 >0) ? Dp1 * ((startLayer2 - Dds)/Htc1) : 0;
    Fdd2 = (Htc2 > 0 && (startLayer3 - Dds)/Htc2 >0) ? Dp2 * ((startLayer3 - Dds)/Htc2) : 0;
    Fdd3 = (Htc3 > 0 && (startLayer4 - Dds)/Htc3 >0) ? Dp3 * ((startLayer4 - Dds)/Htc3) : 0;
    Fdd4 = (Htc4 > 0 && (startLayer5 - Dds)/Htc4 >0) ? Dp4 * ((startLayer5 - Dds)/Htc4) : 0;
    Fdd5 = (Htc5 > 0 && (startLayer6 - Dds)/Htc5 >0) ? Dp5 * ((startLayer6 - Dds)/Htc5) : 0;
    Fdd6 = (Htc6 > 0 && (startLayer7 - Dds)/Htc6 >0) ? Dp6 * ((startLayer7 - Dds)/Htc6) : 0;
    Fdd7 = (Htc7 > 0 && (startLayer8 - Dds)/Htc7 >0) ? Dp7 * ((startLayer8 - Dds)/Htc7) : 0;
    Fdd8 = (Htc8 > 0 && (startLayer9 - Dds)/Htc8 >0) ? Dp8 * ((startLayer9 - Dds)/Htc8) : 0;
    Fdd9 = (Htc9 > 0 && (startLayer10 - Dds)/Htc9 >0) ? Dp9 * ((startLayer10 - Dds)/Htc9) : 0;
    Fdd10 = (Htc10 > 0 && (startLayer11 - Dds)/Htc10 >0) ? Dp10 * ((startLayer11 - Dds)/Htc10) : 0;

}

} //namespace

