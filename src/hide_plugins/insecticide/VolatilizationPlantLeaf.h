/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef INSECTICIDE_VOLATILIZATIONPLANTLEAF_H
#define INSECTICIDE_VOLATILIZATIONPLANTLEAF_H

#include <usbase/model.h>

namespace insecticide {

class VolatilizationPlantLeaf : public UniSim::Model
{
public:
    VolatilizationPlantLeaf(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double VP, Ea, R, Tr, Ta, dosage, inflow, t;

    // Variables
    double ln_ER, kTa, flv, outflow, concentration, kTr;


};

} //namespace


#endif
