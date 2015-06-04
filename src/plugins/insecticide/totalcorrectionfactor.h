/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_TOTALCORRECTIONFACTOR_H
#define INSECTICIDE_TOTALCORRECTIONFACTOR_H

#include <usbase/model.h>

namespace insecticide {

class totalcorrectionfactor : public UniSim::Model
{
public:
    totalcorrectionfactor(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double fsm, fssdT, fba1, fba2, fba3, fba4, fba5,
    fba6, fba7, fba8, fba9, fba10,
    depthCorrPerHorizon1, depthCorrPerHorizon2, depthCorrPerHorizon3, depthCorrPerHorizon4, depthCorrPerHorizon5,
    depthCorrPerHorizon6, depthCorrPerHorizon7, depthCorrPerHorizon8, depthCorrPerHorizon9, depthCorrPerHorizon10;

    // Output
    double tcf1, tcf2,tcf3,
    tcf4, tcf5, tcf6,
    tcf7, tcf8, tcf9,
    tcf10;
};

} //namespace


#endif
