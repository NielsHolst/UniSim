/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_BIOAVAILABILITYCORRECTION_H
#define INSECTICIDE_BIOAVAILABILITYCORRECTION_H

#include <usbase/model.h>

namespace insecticide {

class bioavailabilitycorrection : public UniSim::Model
{
public:
    bioavailabilitycorrection(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double ps, Sp, pHcorrectedSorptionLayer1, pHcorrectedSorptionLayer2, pHcorrectedSorptionLayer3, pHcorrectedSorptionLayer4, pHcorrectedSorptionLayer5,
    pHcorrectedSorptionLayer6, pHcorrectedSorptionLayer7, pHcorrectedSorptionLayer8, pHcorrectedSorptionLayer9, pHcorrectedSorptionLayer10;

    // Output
    double fba1, fba2,fba3,
    fba4, fba5, fba6,
    fba7, fba8, fba9,
    fba10;
};

} //namespace


#endif
