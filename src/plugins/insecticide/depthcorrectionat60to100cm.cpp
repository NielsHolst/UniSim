/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "depthcorrectionat60to100cm.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (depthcorrectionat60to100cm)
	
depthcorrectionat60to100cm::depthcorrectionat60to100cm(Identifier name, QObject *parent)
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

    Output (double, depthCorr60to100cm1); //depths at which different losses occur, in fractions
    Output (double, depthCorr60to100cm2);
    Output (double, depthCorr60to100cm3);
    Output (double, depthCorr60to100cm4);
    Output (double, depthCorr60to100cm5);
    Output (double, depthCorr60to100cm6);
    Output (double, depthCorr60to100cm7);
    Output (double, depthCorr60to100cm8);
    Output (double, depthCorr60to100cm9);
    Output (double, depthCorr60to100cm10);

}

void depthcorrectionat60to100cm::reset() {

    depthCorr60to100cm1 = 0.;
    depthCorr60to100cm2 = 0.;
    depthCorr60to100cm3 = 0.;
    depthCorr60to100cm4 = 0.;
    depthCorr60to100cm5 = 0.;
    depthCorr60to100cm6 = 0.;
    depthCorr60to100cm7 = 0.;
    depthCorr60to100cm8 = 0.;
    depthCorr60to100cm9 = 0.;
    depthCorr60to100cm10 = 0.;

}

void depthcorrectionat60to100cm::update() {

    double x1 = (startLayer2 < 0.601) ? 0 : 1.-((0.6 - startLayer1)/(Htc1));
    if (startLayer1 > 0.601){
        depthCorr60to100cm1 = 1;
    }
    else{
        depthCorr60to100cm1 = x1;
    }

    //...

    double x2 = (startLayer3 < 0.601) ? 0 : 1.-((0.6 - startLayer2)/(Htc2));
    if (startLayer2 > 0.601){
        depthCorr60to100cm2 = 1;
    }
    else{
        depthCorr60to100cm2 = x2;
    }

    //...

    double x3 = (startLayer4 < 0.601) ? 0 : 1.-((0.6 - startLayer3)/(Htc3));
    if (startLayer3 > 0.601){
        depthCorr60to100cm3 = 1;
    }
    else{
        depthCorr60to100cm3 = x3;
    }

    //...

    double x4 = (startLayer5 < 0.601) ? 0 : 1.-((0.6 - startLayer4)/(Htc4));
    if (startLayer4 > 0.601){
        depthCorr60to100cm4 = 1;
    }
    else{
        depthCorr60to100cm4 = x4;
    }

    //...

    double x5 = (startLayer6 < 0.601) ? 0 : 1.-((0.6 - startLayer5)/(Htc5));
    if (startLayer5 > 0.601){
        depthCorr60to100cm5 = 1;
    }
    else{
        depthCorr60to100cm5 = x5;
    }

    //...

    double x6 = (startLayer7 < 0.601) ? 0 : 1.-((0.6 - startLayer6)/(Htc6));
    if (startLayer6 > 0.601){
        depthCorr60to100cm6 = 1;
    }
    else{
        depthCorr60to100cm6 = x6;
    }

    //...

    double x7 = (startLayer8 < 0.601) ? 0 : 1.-((0.6 - startLayer7)/(Htc7));
    if (startLayer7 > 0.601){
        depthCorr60to100cm7 = 1;
    }
    else{
        depthCorr60to100cm7 = x7;
    }

    //...

    double x8 = (startLayer9 < 0.601) ? 0 : 1.-((0.6 - startLayer8)/(Htc8));
    if (startLayer8 > 0.601){
        depthCorr60to100cm8 = 1;
    }
    else{
        depthCorr60to100cm8 = x8;
    }

    //...

    double x9 = (startLayer10 < 0.601) ? 0 : 1.-((0.6 - startLayer9)/(Htc9));
    if (startLayer9 > 0.601){
        depthCorr60to100cm9 = 1;
    }
    else{
        depthCorr60to100cm9 = x9;
    }

    //...

    double x10 = (startLayer11 < 0.601) ? 0 : 1.-((0.6 - startLayer10)/(Htc10));
    if (startLayer10 > 0.601){
        depthCorr60to100cm10 = 1;
    }
    else{
        depthCorr60to100cm10 = x10;
    }

}

} //namespace

