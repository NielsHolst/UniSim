/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VAPOUR_FLUX_TRANSPIRATION_H
#define VG_VAPOUR_FLUX_TRANSPIRATION_H

#include <usbase/model.h>
#include "vapour_flux_base.h"

namespace vg {

class VapourFluxTranspiration : public VapourFluxBase
{
public:
    VapourFluxTranspiration(UniSim::Identifier name, QObject *parent);
    void update();

private:
    double conductanceIn, vapourFluxIn, gainIn;

};
} //namespace


#endif
