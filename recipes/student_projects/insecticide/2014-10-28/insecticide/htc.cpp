/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "htc.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (htc)

htc::htc(Identifier name, QObject *parent)
     : Model(name, parent)
{
    Input (double, penetrationDepth, 0.01);      //default value, 0.01 m

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



void htc::reset() {
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
void htc::update() {    

    Htc1 = pullValue<double>("(Soil1average-startLayer2)")-("(Soil1average-startLayer1)")- penetrationDepth;
    Htc2 = pullValue<double>("(Soil1average-startLayer3)")-("(Soil1average-startLayer2)");
    Htc3 = pullValue<double>("(Soil1average-startLayer4)")-("(Soil1average-startLayer3)");
    Htc4 = pullValue<double>("(Soil1average-startLayer5)")-("(Soil1average-startLayer4)");
    Htc5 = pullValue<double>("(Soil1average-startLayer6)")-("(Soil1average-startLayer5)");
    Htc6 = pullValue<double>("(Soil1average-startLayer7)")-("(Soil1average-startLayer6)");
    Htc7 = pullValue<double>("(Soil1average-startLayer8)")-("(Soil1average-startLayer7)");
    Htc8 = pullValue<double>("(Soil1average-startLayer9)")-("(Soil1average-startLayer8)");
    Htc9 = pullValue<double>("(Soil1average-startLayer10)")-("(Soil1average-startLayer9)");
    Htc10 = pullValue<double>("(Soil1average-startLayer11)")-("(Soil1average-startLayer10)");


}
} //namespace
