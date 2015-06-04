/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "drainagelosses.h"
#include <cmath>
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (drainagelosses)
	
drainagelosses::drainagelosses(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, Rt6, 1.);            //h
    Input (double, Rt2, 0.33);            //h
    Input (double, Rt3, 0.55);            //h
    Input (double, Rt10, 1.);           //h
    Input (double, Rt8, 1.);            //h
    Input (double, Rt7, 1.);            //h
    Input (double, Rt5, 1.);            //h
    Input (double, Df, 0.55);                              //drained fraction default value, 0.55
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

    Input (double, Fdd3, 0.);                      //Htc1 to Htc10 have units of "m"
    Input (double, Fdd2, 0.);
    Input (double, Fdd4, 0.);
    Input (double, Fdd1, 0.);
    Input (double, Fdd5, 0.);
    Input (double, Fdd6, 0.);
    Input (double, Fdd7, 0.);
    Input (double, Fdd8, 0.);
    Input (double, Fdd9, 0.);
    Input (double, Fdd10, 0.);

    //Input (double, Doseafl, 0.); //dose available for leaching (kg a.i/ha)


    Output (double, DLosses3);                      //fraction of drainage losses per horizon, fraction of the incoming pesticide in layer x that is lost to drainage
    Output (double, DLosses2);
    Output (double, DLosses4);
    Output (double, DLosses1);
    Output (double, DLosses5);
    Output (double, DLosses6);
    Output (double, DLosses7);
    Output (double, DLosses8);
    Output (double, DLosses9);
    Output (double, DLosses10);

}

void drainagelosses::reset() {
    DLosses3 = 0.;
    DLosses2 = 0.;
    DLosses4 = 0.;
    DLosses5 = 0.;
    DLosses6 = 0.;
    DLosses7 = 0.;
    DLosses1 = 0.;
    DLosses8 = 0.;
    DLosses9 = 0.;
    DLosses10 = 0.;

}

void drainagelosses::update() {

    DLosses1 = (Fdd1 == 0) ? 0: Df * exp(-k1 * (1. - Fdd1) * Rt1);
    DLosses2 = (Fdd2 == 0) ? 0: Df * exp(-k2 * (1. - Fdd2) * Rt2);
    DLosses3 = (Fdd3 == 0) ? 0: Df * exp(-k3 * (1. - Fdd3)*(Rt3));
    DLosses4 = (Fdd4 == 0) ? 0: Df * exp(-k4 * (1. - Fdd4) * Rt4);
    DLosses5 = (Fdd5 == 0) ? 0: Df * exp(-k5 * (1. - Fdd5) * Rt5);
    DLosses6 = (Fdd6 == 0) ? 0: Df * exp(-k6 * (1. - Fdd6) * Rt6);
    DLosses7 = (Fdd7 == 0) ? 0: Df * exp(-k7 * (1. - Fdd7) * Rt7);
    DLosses8 = (Fdd8 == 0) ? 0: Df * exp(-k8 * (1. - Fdd8) * Rt8);
    DLosses9 = (Fdd9 == 0) ? 0: Df * exp(-k9 * (1. - Fdd9) * Rt9);
    DLosses10 = (Fdd10 == 0) ? 0: Df * exp(-k10 * (1. - Fdd10) * Rt10);


//    drainageLosses1 = (Fdd1 == 0) ? 0: Doseafl*Df * exp(-k1 * (1. - Fdd1) * retardationTimeLayer1);
//    drainageLosses2 = (Fdd2 == 0) ? 0: Doseafl*Df * exp(-k2 * (1. - Fdd2) * retardationTimeLayer2);
//    drainageLosses3 = (Fdd3 == 0) ? 0: Doseafl*Df*exp(-(k3)*(1.-Fdd3)*(retardationTimeLayer3));
//    drainageLosses4 = (Fdd4 == 0) ? 0: Doseafl*Df * exp(-k4 * (1. - Fdd4) * retardationTimeLayer4);
//    drainageLosses5 = (Fdd5 == 0) ? 0: Doseafl*Df * exp(-k5 * (1. - Fdd5) * retardationTimeLayer5);
//    drainageLosses6 = (Fdd6 == 0) ? 0: Doseafl*Df * exp(-k6 * (1. - Fdd6) * retardationTimeLayer6);
//    drainageLosses7 = (Fdd7 == 0) ? 0: Doseafl*Df * exp(-k7 * (1. - Fdd7) * retardationTimeLayer7);
//    drainageLosses8 = (Fdd8 == 0) ? 0: Doseafl*Df * exp(-k8 * (1. - Fdd8) * retardationTimeLayer8);
//    drainageLosses9 = (Fdd9 == 0) ? 0: Doseafl*Df * exp(-k9 * (1. - Fdd9) * retardationTimeLayer9);
//    drainageLosses10 = (Fdd10 == 0) ? 0: Doseafl*Df * exp(-k10 * (1. - Fdd10) * retardationTimeLayer10);

}

} //namespace

