/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_DEPTHCORRECTIONPERHORIZON_H
#define INSECTICIDE_DEPTHCORRECTIONPERHORIZON_H

#include <usbase/model.h>

namespace insecticide {

class depthcorrectionperhorizon : public UniSim::Model
{
public:
    depthcorrectionperhorizon(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double depthCorr0to30cm6, depthCorr0to30cm3, depthCorr0to30cm2, depthCorr0to30cm10,
    depthCorr0to30cm8, depthCorr0to30cm7, depthCorr0to30cm9, depthCorr0to30cm5,
    depthCorr0to30cm1, depthCorr0to30cm4, depthCorr30to60cm3, depthCorr30to60cm2,
    depthCorr30to60cm4, depthCorr30to60cm1, depthCorr30to60cm5, depthCorr30to60cm6,
    depthCorr30to60cm7, depthCorr30to60cm8, depthCorr30to60cm9, depthCorr30to60cm10,
    depthCorr60to100cm1, depthCorr60to100cm2, depthCorr60to100cm3, depthCorr60to100cm4,
    depthCorr60to100cm5, depthCorr60to100cm6, depthCorr60to100cm7, depthCorr60to100cm8,
    depthCorr60to100cm9, depthCorr60to100cm10;

    // Output
    double depthCorrPerHorizon1, depthCorrPerHorizon2, depthCorrPerHorizon3, depthCorrPerHorizon4,
    depthCorrPerHorizon5, depthCorrPerHorizon6, depthCorrPerHorizon7, depthCorrPerHorizon8,
    depthCorrPerHorizon9, depthCorrPerHorizon10;
};

} //namespace


#endif
