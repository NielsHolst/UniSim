/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_MASS_H
#define VG_CROP_MASS_H

#include <usbase/model.h>

namespace vg {

class CropMass : public UniSim::Model
{
public:
    CropMass(UniSim::Identifier name, QObject *parent);
    void reset();
    void update() final;
    virtual void setProportions() = 0;
protected:
    double initMass, grossGrowthRate,
        propRoot, propStem, propLeaf, propFruit,
        costRoot, costStem, costLeaf, costFruit,
        root, stem, leaf, fruit, total;
    bool establishCrop, removeCrop;
    void allocate();
private:
    void establish();
    void remove();
};
} //namespace


#endif
