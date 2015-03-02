/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_FRUIT_CROP_MASS_H
#define VG_FRUIT_CROP_MASS_H

#include "crop_mass.h"

namespace vg {

class FruitCropMass : public CropMass
{
public:
    FruitCropMass(UniSim::Identifier name, QObject *parent);
    void setProportions();

private:
    double indoorsTemperature, tempSum, maxLai, fruitFactor,
        tempSumReproductive, tempMaxReproductive, lai,
        vegPropRoot, vegPropStem, vegPropLeaf, vegPropFruit,
        reproPropRoot, reproPropStem, reproPropLeaf, reproPropFruit;
    bool maxLaiReached;
    void reallocateToFruits();
};
} //namespace


#endif
