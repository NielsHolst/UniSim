/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef XPEST_CROP_COMPONENT_H
#define XPEST_CROP_COMPONENT_H

#include <usbase/model.h>

namespace xpest {

class CropComponent : public UniSim::Model
{
public:
    CropComponent(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    // Parameters
    double assimilate, assimilateLoss, sla, initMass, senescence,
        remobilizationIn, remobilizationOut,
        partitioningCoefficient;
    bool isBelowGround;

    // Variables
    double netPartition, mass, lai;

    // Links
    const double *belowGroundPartition;

    // Methods
    void updateFromMass();
};

} //namespace


#endif
