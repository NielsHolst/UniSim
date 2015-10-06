/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BOUNDARY_LAYER_RESISTANCE_THORNLEY_JOHNSEN_H
#define VG_BOUNDARY_LAYER_RESISTANCE_THORNLEY_JOHNSEN_H

#include "boundary_layer_resistance_base.h"

namespace vg {

class BoundaryLayerResistanceThornleyJohnsen : public BoundaryLayerResistanceBase
{
public:
    BoundaryLayerResistanceThornleyJohnsen(UniSim::Identifier name, QObject *parent);
    void reset();
    void setRbH2O();
private:
    // Inputs
    double leafTemperature, indoorsTemperature;
    // Outputs
    double gbH2Ofree, gbH2Oforced;
};

} //namespace


#endif
