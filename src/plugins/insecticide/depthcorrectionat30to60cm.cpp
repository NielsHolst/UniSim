/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "depthcorrectionat30to60cm.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (depthcorrectionat30to60cm)
	
depthcorrectionat30to60cm::depthcorrectionat30to60cm(Identifier name, QObject *parent)
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
    Input (double, startLayer1, 0.);             //sm
    Input (double, startLayer4, 0.8);             //m
    Input (double, startLayer9, 1.);             //m
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

    Output (double, depthCorr30to60cm1); //depths at which different losses occur, in fractions
    Output (double, depthCorr30to60cm2);
    Output (double, depthCorr30to60cm3);
    Output (double, depthCorr30to60cm4);
    Output (double, depthCorr30to60cm5);
    Output (double, depthCorr30to60cm6);
    Output (double, depthCorr30to60cm7);
    Output (double, depthCorr30to60cm8);
    Output (double, depthCorr30to60cm9);
    Output (double, depthCorr30to60cm10);

}

void depthcorrectionat30to60cm::reset() {

    depthCorr30to60cm1 = 0.;
    depthCorr30to60cm2 = 0.;
    depthCorr30to60cm3 = 0.;
    depthCorr30to60cm4 = 0.;
    depthCorr30to60cm5 = 0.;
    depthCorr30to60cm6 = 0.;
    depthCorr30to60cm7 = 0.;
    depthCorr30to60cm8 = 0.;
    depthCorr30to60cm9 = 0.;
    depthCorr30to60cm10 = 0.;

}

void depthcorrectionat30to60cm::update() {

    double x1 = ((((startLayer2 - 0.6))/Htc1) < 0) ? 0 : (((startLayer2 - 0.6))/Htc1);
    double x2 = ((0.3 - (startLayer1))/Htc1 < 0) ? 0 : (0.3 - (startLayer1))/Htc1;
    double x3 = (startLayer1 > 0.60) ? 0 : 1. - x2 - x1;
    if (startLayer2 > 0.30){
        depthCorr30to60cm1 = x3;
    }
    else{
        depthCorr30to60cm1 = 0;
    }

    //.....

    double x11 = ((((startLayer3 - 0.6))/Htc2) < 0) ? 0 : (((startLayer3 - 0.6))/Htc2);
    double x22 = ((0.3 - (startLayer2))/Htc2 < 0) ? 0 : (0.3 - (startLayer2))/Htc2;
    double x33 = (startLayer2 > 0.60) ? 0 : 1. - x22 - x11;
    if (startLayer3 > 0.30){
        depthCorr30to60cm2 = x33;
    }
    else{
        depthCorr30to60cm2 = 0;
    }

     //.....

    double x111 = ((((startLayer4 - 0.6))/Htc3) < 0) ? 0 : (((startLayer4 - 0.6))/Htc3);
    double x222 = ((0.3 -(startLayer3))/Htc3 < 0) ? 0 : (0.3 - (startLayer3))/Htc3;
    double x333 = (startLayer3 > 0.60) ? 0 : 1.- x222 - x111;
    if (startLayer4 > 0.30){
        depthCorr30to60cm3 = x333;
    }
    else{
        depthCorr30to60cm3 = 0;
    }

     //.....

    double x1111 = ((((startLayer5 - 0.6))/Htc4) < 0) ? 0 : (((startLayer5 - 0.6))/Htc4);
    double x2222 = ((0.3 -(startLayer4))/Htc4 < 0) ? 0 : (0.3 - (startLayer4))/Htc4;
    double x3333 = (startLayer4 > 0.60) ? 0 : 1.- x2222 - x1111;
    if (startLayer5 > 0.30){
        depthCorr30to60cm4 = x3333;
    }
    else{
        depthCorr30to60cm4 = 0;
    }

     //.....

    double x11111 = ((((startLayer6 - 0.6))/Htc5) < 0) ? 0 : (((startLayer6 - 0.6))/Htc5);
    double x22222 = ((0.3 -(startLayer5))/Htc5 < 0) ? 0 : (0.3 - (startLayer5))/Htc5;
    double x33333 = (startLayer5 > 0.60) ? 0 : 1.- x22222 - x11111;
    if (startLayer6 > 0.30){
        depthCorr30to60cm5 = x33333;
    }
    else{
        depthCorr30to60cm5 = 0;
    }

     //.....

    double x111111 = ((((startLayer7 - 0.6))/Htc6) < 0) ? 0 : (((startLayer7 - 0.6))/Htc6);
    double x222222 = ((0.3 -(startLayer6))/Htc6 < 0) ? 0 : (0.3 - (startLayer6))/Htc6;
    double x333333 = (startLayer6 > 0.60) ? 0 : 1.- x222222 - x111111;
    if (startLayer7 > 0.30){
        depthCorr30to60cm6 = x333333;
    }
    else{
        depthCorr30to60cm6 = 0;
    }

     //.....

    double x1111111 = ((((startLayer8 - 0.6))/Htc7) < 0) ? 0 : (((startLayer8 - 0.6))/Htc7);
    double x2222222 = ((0.3 -(startLayer7))/Htc7 < 0) ? 0 : (0.3 - (startLayer7))/Htc7;
    double x3333333 = (startLayer7 > 0.60) ? 0 : 1.- x2222222 - x1111111;
    if (startLayer8 > 0.30){
        depthCorr30to60cm7 = x3333333;
    }
    else{
        depthCorr30to60cm7 = 0;
    }

     //.....

    double x11111111 = ((((startLayer9 - 0.6))/Htc8) < 0) ? 0 : (((startLayer9 - 0.6))/Htc8);
    double x22222222 = ((0.3 -(startLayer8))/Htc8 < 0) ? 0 : (0.3 - (startLayer8))/Htc8;
    double x33333333 = (startLayer8 > 0.60) ? 0 : 1.- x22222222 - x11111111;
    if (startLayer9 > 0.30){
        depthCorr30to60cm8 = x33333333;
    }
    else{
        depthCorr30to60cm8 = 0;
    }

     //.....

    double x111111111 = ((((startLayer10 - 0.6))/Htc9) < 0) ? 0 : (((startLayer10 - 0.6))/Htc9);
    double x222222222 = ((0.3 -(startLayer9))/Htc9 < 0) ? 0 : (0.3 - (startLayer9))/Htc9;
    double x333333333 = (startLayer9 > 0.60) ? 0 : 1.- x222222222 - x111111111;
    if (startLayer10 > 0.30){
        depthCorr30to60cm9 = x333333333;
    }
    else{
        depthCorr30to60cm9 = 0;
    }

     //.....

    double x1111111111 = ((((startLayer11 - 0.6))/Htc10) < 0) ? 0 : (((startLayer11 - 0.6))/Htc10);
    double x2222222222 = ((0.3 -(startLayer10))/Htc10 < 0) ? 0 : (0.3 - (startLayer10))/Htc10;
    double x3333333333 = (startLayer10 > 0.60) ? 0 : 1.- x2222222222 - x1111111111;
    if (startLayer11 > 0.30){
        depthCorr30to60cm10 = x3333333333;
    }
    else{
        depthCorr30to60cm10 = 0;
    }

}

} //namespace

