/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BOUNDARY_LAYER_RESISTANCE_JONES_B_H
#define VG_BOUNDARY_LAYER_RESISTANCE_JONES_B_    boundary_layer_resistance_jones.cpp \
H

#include "boundary_layer_resistance_base.h"

namespace vg {

class BoundaryLayerResistanceJonesB : public BoundaryLayerResistanceBase
{
public:
    BoundaryLayerResistanceJonesB(UniSim::Identifier name, QObject *parent);
    void reset();
    void setRbH2O();
private:
    // Inputs
    double evaporation, leafTemperature, indoorsTemperature, indoorsRh;
};
} //namespace


#endif
