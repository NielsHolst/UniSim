/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "depthcorrectionat0to30cm.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (depthcorrectionat0to30cm)
	
depthcorrectionat0to30cm::depthcorrectionat0to30cm(Identifier name, QObject *parent)
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

    Output (double, depthCorr0to30cm1); //depths at which different losses occur, in fractions
    Output (double, depthCorr0to30cm2);
    Output (double, depthCorr0to30cm3);
    Output (double, depthCorr0to30cm4);
    Output (double, depthCorr0to30cm5);
    Output (double, depthCorr0to30cm6);
    Output (double, depthCorr0to30cm7);
    Output (double, depthCorr0to30cm8);
    Output (double, depthCorr0to30cm9);
    Output (double, depthCorr0to30cm10);

}

void depthcorrectionat0to30cm::reset() {

    depthCorr0to30cm1 = 0.;
    depthCorr0to30cm2 = 0.;
    depthCorr0to30cm3 = 0.;
    depthCorr0to30cm4 = 0.;
    depthCorr0to30cm5 = 0.;
    depthCorr0to30cm6 = 0.;
    depthCorr0to30cm7 = 0.;
    depthCorr0to30cm8 = 0.;
    depthCorr0to30cm9 = 0.;
    depthCorr0to30cm10 = 0.;

}

void depthcorrectionat0to30cm::update() {

    double x1 = (startLayer1 > 0.3) ? 0 : (1. - (startLayer2 - 0.30)/Htc1);
    if (startLayer2 < 0.3){
        depthCorr0to30cm1 = 1;
    }
    else{
        depthCorr0to30cm1 = x1;
    }

    double x2 = (startLayer2 > 0.3) ? 0 : (1. - (startLayer3 - 0.30)/Htc2);
    if (startLayer3 < 0.3){
        depthCorr0to30cm2 = 1;
    }
    else{
        depthCorr0to30cm2 = x2;
    }

    double x3 = (startLayer3 > 0.3) ? 0 : (1. - (startLayer4 - 0.30)/Htc3);
    if (startLayer4 < 0.3){
        depthCorr0to30cm3 = 1;
    }
    else{
        depthCorr0to30cm3 = x3;
    }

    double x4 = (startLayer4 > 0.3) ? 0 : (1. - (startLayer5 - 0.30)/Htc4);
    if (startLayer5 < 0.3){
        depthCorr0to30cm4 = 1;
    }
    else{
        depthCorr0to30cm4 = x4;
    }

    double x5 = (startLayer5 > 0.3) ? 0 : (1. - (startLayer6 - 0.30)/Htc5);
    if (startLayer6 < 0.3){
        depthCorr0to30cm5 = 1;
    }
    else{
        depthCorr0to30cm5 = x5;
    }

    double x6 = (startLayer6 > 0.3) ? 0 : (1. - (startLayer7 - 0.30)/Htc6);
    if (startLayer7 < 0.3){
        depthCorr0to30cm6 = 1;
    }
    else{
        depthCorr0to30cm6 = x6;
    }

    double x7 = (startLayer7 > 0.3) ? 0 : (1. - (startLayer8 - 0.30)/Htc7);
    if (startLayer8 < 0.3){
        depthCorr0to30cm7 = 1;
    }
    else{
        depthCorr0to30cm7 = x7;
    }

    double x8 = (startLayer8 > 0.3) ? 0 : (1. - (startLayer9 - 0.30)/Htc8);
    if (startLayer9 < 0.3){
        depthCorr0to30cm8 = 1;
    }
    else{
        depthCorr0to30cm8 = x8;
    }

    double x9 = (startLayer9 > 0.3) ? 0 : (1. - (startLayer10 - 0.30)/Htc9);
    if (startLayer10 < 0.3){
        depthCorr0to30cm9 = 1;
    }
    else{
        depthCorr0to30cm9 = x9;
    }

    double x10 = (startLayer10 > 0.3) ? 0 : (1. - (startLayer11 - 0.30)/Htc10);
    if (startLayer11 < 0.3){
        depthCorr0to30cm10 = 1;
    }
    else{
        depthCorr0to30cm10 = x10;
    }

}

} //namespace

