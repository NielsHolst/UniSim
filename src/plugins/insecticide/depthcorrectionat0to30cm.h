/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_DEPTHCORRECTIONAT0TO30CM_H
#define INSECTICIDE_DEPTHCORRECTIONAT0TO30CM_H

#include <usbase/model.h>

namespace insecticide {

class depthcorrectionat0to30cm : public UniSim::Model
{
public:
    depthcorrectionat0to30cm(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double startLayer6, startLayer3, startLayer2, startLayer10,
    startLayer11, startLayer8, startLayer7, startLayer9, startLayer5,
    startLayer1, startLayer4, Htc3, Htc2, Htc4, Htc1, Htc5, Htc6, Htc7, Htc8, Htc9, Htc10;

    // Output
    double depthCorr0to30cm1, depthCorr0to30cm2, depthCorr0to30cm3, depthCorr0to30cm4,
    depthCorr0to30cm5, depthCorr0to30cm6, depthCorr0to30cm7, depthCorr0to30cm8,
    depthCorr0to30cm9, depthCorr0to30cm10;
};

} //namespace


#endif
