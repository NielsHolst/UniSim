/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "depthcorrectionperhorizon.h"
#include "publish.h"

using namespace UniSim;

namespace insecticide {

PUBLISH (depthcorrectionperhorizon)
	
depthcorrectionperhorizon::depthcorrectionperhorizon(Identifier name, QObject *parent)
	: Model(name, parent)
{

    Input (double, depthCorr0to30cm6, 1.);
    Input (double, depthCorr0to30cm2, 0.33);
    Input (double, depthCorr0to30cm3, 0.55);
    Input (double, depthCorr0to30cm10, 1.);
    Input (double, depthCorr0to30cm8, 1.);
    Input (double, depthCorr0to30cm7, 1.);
    Input (double, depthCorr0to30cm5, 1.);
    Input (double, depthCorr0to30cm1, 0.);
    Input (double, depthCorr0to30cm4, 0.8);
    Input (double, depthCorr0to30cm9, 1.);

    Input (double, depthCorr30to60cm3, 0.);
    Input (double, depthCorr30to60cm2, 0.);
    Input (double, depthCorr30to60cm4, 0.);
    Input (double, depthCorr30to60cm1, 0.);
    Input (double, depthCorr30to60cm5, 0.);
    Input (double, depthCorr30to60cm6, 0.);
    Input (double, depthCorr30to60cm7, 0.);
    Input (double, depthCorr30to60cm8, 0.);
    Input (double, depthCorr30to60cm9, 0.);
    Input (double, depthCorr30to60cm10, 0.);

    Input (double, depthCorr60to100cm3, 0.);
    Input (double, depthCorr60to100cm2, 0.);
    Input (double, depthCorr60to100cm4, 0.);
    Input (double, depthCorr60to100cm1, 0.);
    Input (double, depthCorr60to100cm5, 0.);
    Input (double, depthCorr60to100cm6, 0.);
    Input (double, depthCorr60to100cm7, 0.);
    Input (double, depthCorr60to100cm8, 0.);
    Input (double, depthCorr60to100cm9, 0.);
    Input (double, depthCorr60to100cm10, 0.);

    Output (double, depthCorrPerHorizon1);
    Output (double, depthCorrPerHorizon2);
    Output (double, depthCorrPerHorizon3);
    Output (double, depthCorrPerHorizon4);
    Output (double, depthCorrPerHorizon5);
    Output (double, depthCorrPerHorizon6);
    Output (double, depthCorrPerHorizon7);
    Output (double, depthCorrPerHorizon8);
    Output (double, depthCorrPerHorizon9);
    Output (double, depthCorrPerHorizon10);

}

void depthcorrectionperhorizon::reset() {

    depthCorrPerHorizon1 = 0.;
    depthCorrPerHorizon2 = 0.;
    depthCorrPerHorizon3 = 0.;
    depthCorrPerHorizon4 = 0.;
    depthCorrPerHorizon5 = 0.;
    depthCorrPerHorizon6 = 0.;
    depthCorrPerHorizon7 = 0.;
    depthCorrPerHorizon8 = 0.;
    depthCorrPerHorizon9 = 0.;
    depthCorrPerHorizon10 = 0.;

}

void depthcorrectionperhorizon::update() {

    depthCorrPerHorizon1 = (depthCorr0to30cm1 * 1.) + (depthCorr30to60cm1 * 0.5) + (depthCorr60to100cm1 * 0.3);
    depthCorrPerHorizon2 = (depthCorr0to30cm2 * 1.) + (depthCorr30to60cm2 * 0.5) + (depthCorr60to100cm2 * 0.3);
    depthCorrPerHorizon3 = (depthCorr0to30cm3 * 1.) + (depthCorr30to60cm3 * 0.5) + (depthCorr60to100cm3 * 0.3);
    depthCorrPerHorizon4 = (depthCorr0to30cm4 * 1.) + (depthCorr30to60cm4 * 0.5) + (depthCorr60to100cm4 * 0.3);
    depthCorrPerHorizon5 = (depthCorr0to30cm5 * 1.) + (depthCorr30to60cm5 * 0.5) + (depthCorr60to100cm5 * 0.3);
    depthCorrPerHorizon6 = (depthCorr0to30cm6 * 1.) + (depthCorr30to60cm6 * 0.5) + (depthCorr60to100cm6 * 0.3);
    depthCorrPerHorizon7 = (depthCorr0to30cm7 * 1.) + (depthCorr30to60cm7 * 0.5) + (depthCorr60to100cm7 * 0.3);
    depthCorrPerHorizon8 = (depthCorr0to30cm8 * 1.) + (depthCorr30to60cm8 * 0.5) + (depthCorr60to100cm8 * 0.3);
    depthCorrPerHorizon9 = (depthCorr0to30cm9 * 1.) + (depthCorr30to60cm9 * 0.5) + (depthCorr60to100cm9 * 0.3);
    depthCorrPerHorizon10 = (depthCorr0to30cm10 * 1.) + (depthCorr30to60cm10 * 0.5) + (depthCorr60to100cm10 * 0.3);

}

} //namespace

