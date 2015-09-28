/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_FLOOR_RADIATION_ABSORBED_H
#define VG_FLOOR_RADIATION_ABSORBED_H

#include <usbase/model.h>

namespace vg {

class FloorRadiationAbsorbed : public UniSim::Model
{
public:
    FloorRadiationAbsorbed(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double indoorsLight, growthLightLight, lightAbsorbedByCrop,
        growthLightLw, growthLightLwAbsorbedByCrop;

    // Outputs
    double value;
};

} //namespace

#endif
