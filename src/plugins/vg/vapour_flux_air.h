/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VAPOUR_FLUX_AIR_H
#define VG_VAPOUR_FLUX_AIR_H

#include "vapour_flux_base.h"

namespace vg {

class VapourFluxAir : public VapourFluxBase
{
public:
    VapourFluxAir(UniSim::Identifier name, QObject *parent);
    void update();
private:
    // Inputs
    double airFlux, donorAh, receiverAh, height;
};
} //namespace


#endif
