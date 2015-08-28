/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_LIGHT_ABSORBED_H
#define VG_CROP_LIGHT_ABSORBED_H

#include <usbase/model.h>

namespace vg {

class CropLightAbsorbed : public UniSim::Model
{
public:
    CropLightAbsorbed(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double parAbsorbed, lightAbsorptionCoefficient;
    // Outputs
    double value;
};
} //namespace


#endif
