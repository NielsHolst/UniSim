/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_DEPTHCORRECTIONAT30TO60CM_H
#define INSECTICIDE_DEPTHCORRECTIONAT30TO60CM_H

#include <usbase/model.h>

namespace insecticide {

class depthcorrectionat30to60cm : public UniSim::Model
{
public:
    depthcorrectionat30to60cm(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double startLayer6, startLayer3, startLayer2, startLayer10,
    startLayer11, startLayer8, startLayer7, startLayer9, startLayer5,
    startLayer1, startLayer4, Htc3, Htc2, Htc4, Htc1, Htc5, Htc6, Htc7, Htc8, Htc9, Htc10;

    // Output
    double depthCorr30to60cm1, depthCorr30to60cm2, depthCorr30to60cm3, depthCorr30to60cm4,
    depthCorr30to60cm5, depthCorr30to60cm6, depthCorr30to60cm7, depthCorr30to60cm8,
    depthCorr30to60cm9, depthCorr30to60cm10;
};

} //namespace


#endif
