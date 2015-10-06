/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BOUNDARY_LAYER_RESISTANCE_STANGEHELLINI_H
#define VG_BOUNDARY_LAYER_RESISTANCE_STANGEHELLINI_H

#include "boundary_layer_resistance_base.h"

namespace vg {

class BoundaryLayerResistanceStangehellini : public BoundaryLayerResistanceBase
{
public:
    BoundaryLayerResistanceStangehellini(UniSim::Identifier name, QObject *parent);
    void setRbH2O();
    // Inputs
    double leafTemperature, indoorsTemperature;
};
} //namespace


#endif
