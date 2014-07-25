/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CROP_TRANSPIRATION_H
#define VG_CROP_TRANSPIRATION_H

#include <usbase/model.h>
#include "vapour_flux.h"

namespace vg {

class CropTranspiration : public VapourFlux
{
public:
    CropTranspiration(UniSim::Identifier name, QObject *parent);
    void update();

private:
    double ConductanceIn, VapourFluxIn, GainIn;

};
} //namespace


#endif
