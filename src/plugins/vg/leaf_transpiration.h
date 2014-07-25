/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LEAF_TRANSPIRATION_H
#define VG_LEAF_TRANSPIRATION_H

#include <usbase/model.h>
#include "vapour_flux.h"

namespace vg {

class LeafTranspiration : public VapourFlux
{
public:
    LeafTranspiration(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    double absorbedRadiation, rsH2O, rbH2O,
        value, indoorsAh, Tleaf, lai, fractionPlantArea, xGauss, wGauss;
};
} //namespace


#endif
