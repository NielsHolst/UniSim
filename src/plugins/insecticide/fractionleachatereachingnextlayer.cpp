/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "fractionleachatereachingnextlayer.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (fractionleachatereachingnextlayer)
	
fractionleachatereachingnextlayer::fractionleachatereachingnextlayer(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Rt6, 1.);            //h
    Input (double, Rt2, 0.33);            //h
    Input (double, Rt3, 0.55);            //h
    Input (double, Rt10, 1.);           //h
    Input (double, Rt8, 1.);            //h
    Input (double, Rt7, 1.);            //h
    Input (double, Rt5, 1.);            //h
    Input (double, Df, 0.55);                              //drained fraction default value, 0.55 m
    Input (double, Rt1, 0.);             //h
    Input (double, Rt4, 0.8);             //h
    Input (double, Rt9, 1.);             //h

    Input (double, k3, 0.);                      //biodegradation rate constant per horizon, h-1
    Input (double, k2, 0.);
    Input (double, k4, 0.);
    Input (double, k1, 0.);
    Input (double, k5, 0.);
    Input (double, k6, 0.);
    Input (double, k7, 0.);
    Input (double, k8, 0.);
    Input (double, k9, 0.);
    Input (double, k10, 0.);

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
    //Input (double, Doseafl, 0.8615);


    Output (double, Fn3);                      //fraction leachate reaching next layer
    Output (double, Fn2);
    Output (double, Fn4);
    Output (double, Fn1);
    Output (double, Fn5);
    Output (double, Fn6);
    Output (double, Fn7);
    Output (double, Fn8);
    Output (double, Fn9);
    Output (double, Fn10);

}

void fractionleachatereachingnextlayer::reset() {
    Fn3 = 0.; //change this to Dn3, ...etc
    Fn2 = 0.;
    Fn4 = 0.;
    Fn5 = 0.;
    Fn6 = 0.;
    Fn7 = 0.;
    Fn1 = 0.;
    Fn8 = 0.;
    Fn9 = 0.;
    Fn10 = 0.;

}

void fractionleachatereachingnextlayer::update() {

//    Fn1 = Doseafl - (Doseafl*Dp1 * Df * exp(-k1 * retardationTimeLayer1));
//    Fn2 = Doseafl - (Doseafl*Dp2 * Df * exp(-k2 * retardationTimeLayer2));
//    Fn3 = Doseafl - (Doseafl*Dp3 * Df * exp(-k3 * retardationTimeLayer3));
//    Fn4 = Doseafl - (Doseafl*Dp4 * Df * exp(-k4 * retardationTimeLayer4));
//    Fn5 = Doseafl - (Doseafl*Dp5 * Df * exp(-k5 * retardationTimeLayer5));
//    Fn6 = Doseafl - (Doseafl*Dp6 * Df * exp(-k6 * retardationTimeLayer6));
//    Fn7 = Doseafl - (Doseafl*Dp7 * Df * exp(-k7 * retardationTimeLayer7));
//    Fn8 = Doseafl - (Doseafl*Dp8 * Df * exp(-k8 * retardationTimeLayer8));
//    Fn9 = Doseafl - (Doseafl*Dp9 * Df * exp(-k9 * retardationTimeLayer9));
//    Fn10 = Doseafl -(Doseafl*Dp10 * Df * exp(-k10 * retardationTimeLayer10));

//    (1-(Dp1*Df))*exp(-k1*retardationTimeLayer1);

    Fn1 = (1. - (Dp1 * Df)) * exp(-k1 * Rt1);
    Fn2 = (1. - (Dp2 * Df)) * exp(-k2 * Rt2);
    Fn3 = (1. - (Dp3 * Df)) * exp(-k3 * Rt3);
    Fn4 = (1. - (Dp4 * Df)) * exp(-k4 * Rt4);
    Fn5 = (1. - (Dp5 * Df)) * exp(-k5 * Rt5);
    Fn6 = (1. - (Dp6 * Df)) * exp(-k6 * Rt6);
    Fn7 = (1. - (Dp7 * Df)) * exp(-k7 * Rt7);
    Fn8 = (1. - (Dp8 * Df)) * exp(-k8 * Rt8);
    Fn9 = (1. - (Dp9 * Df)) * exp(-k9 * Rt9);
    Fn10 = (1. - (Dp10 * Df)) * exp(-k10 * Rt10);

}

} //namespace

