/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_RETARDATIONFACTOR_H
#define INSECTICIDE_RETARDATIONFACTOR_H

#include <usbase/model.h>

namespace insecticide {

class retardationfactor : public UniSim::Model
{
public:
    retardationfactor(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    double pb, fw, pHcorrectedSorptionLayer1, pHcorrectedSorptionLayer2, pHcorrectedSorptionLayer3, pHcorrectedSorptionLayer4, pHcorrectedSorptionLayer5,
    pHcorrectedSorptionLayer6, pHcorrectedSorptionLayer7, pHcorrectedSorptionLayer8, pHcorrectedSorptionLayer9, pHcorrectedSorptionLayer10;

    // Output
    double Rf1, Rf2,Rf3,
    Rf4, Rf5, Rf6,
    Rf7, Rf8, Rf9,
    Rf10;
};

} //namespace


#endif
