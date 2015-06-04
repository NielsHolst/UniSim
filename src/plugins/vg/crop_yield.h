/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_YIELD_H
#define VG_CROP_YIELD_H

#include <usbase/model.h>

namespace vg {

class CropYield : public UniSim::Model
{
public:
    CropYield(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
protected:
    // Inputs
    double produceMass, fractionPlantArea, fractionDryWeight;
    // Outputs
    double freshWeight, dryWeight;
};
} //namespace


#endif
