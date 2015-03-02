/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_FRUIT_CROP_LAI_H
#define VG_FRUIT_CROP_LAI_H

#include "crop_lai.h"

namespace vg {

class FruitCropLai : public CropLai
{
public:
    FruitCropLai(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    double cropDensity, tempSum,
        laiStart, maxLai;
    bool cropPresent, maxLaiReached;
};
} //namespace


#endif
