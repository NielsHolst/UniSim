/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef APHID_EVEN_ODD_H
#define APHID_EVEN_ODD_H

#include <usbase/model.h>

namespace aphid {

class AphidFecundity : public UniSim::Model
{
public:
    AphidFecundity(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double maxFecundity, parentDensity, parentDensityThreshold, dayDegreeTimeStep,
        vegetativePlants, pre_floweringPlants, floweringPlants, maturePlants;

    // Variables
    double newBorns;
};

} //namespace


#endif
