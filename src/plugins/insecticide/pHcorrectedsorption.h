/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_PHCORRECTEDSORPTION_H
#define INSECTICIDE_PHCORRECTEDSORPTION_H

#include <usbase/model.h>

namespace insecticide {

class pHcorrectedsorption : public UniSim::Model
{
public:
    pHcorrectedsorption(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double Koc, pH, pKa, focLayer1, focLayer2, focLayer3, focLayer4, focLayer5,
    focLayer6, focLayer7, focLayer8, focLayer9, focLayer10, pHLayer1, pHLayer2,
    pHLayer3, pHLayer4, pHLayer5, pHLayer6, pHLayer7, pHLayer8, pHLayer9, pHLayer10;

    // Output
    double pHcorrectedSorptionLayer1, pHcorrectedSorptionLayer2,pHcorrectedSorptionLayer3,
    pHcorrectedSorptionLayer4, pHcorrectedSorptionLayer5, pHcorrectedSorptionLayer6,
    pHcorrectedSorptionLayer7, pHcorrectedSorptionLayer8, pHcorrectedSorptionLayer9,
    pHcorrectedSorptionLayer10;
};

} //namespace


#endif
