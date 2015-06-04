/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_DEPTHCORRECTIONAT60TO100CM_H
#define INSECTICIDE_DEPTHCORRECTIONAT60TO100CM_H

#include <usbase/model.h>

namespace insecticide {

class depthcorrectionat60to100cm : public UniSim::Model
{
public:
    depthcorrectionat60to100cm(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double startLayer6, startLayer3, startLayer2, startLayer10,
    startLayer11, startLayer8, startLayer7, startLayer9, startLayer5,
    startLayer1, startLayer4, Htc3, Htc2, Htc4, Htc1, Htc5, Htc6, Htc7, Htc8, Htc9, Htc10;

    // Output
    double depthCorr60to100cm1, depthCorr60to100cm2, depthCorr60to100cm3, depthCorr60to100cm4,
    depthCorr60to100cm5, depthCorr60to100cm6, depthCorr60to100cm7, depthCorr60to100cm8,
    depthCorr60to100cm9, depthCorr60to100cm10;
};

} //namespace


#endif
