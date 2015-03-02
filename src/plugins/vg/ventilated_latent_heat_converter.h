/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VENTILATED_LATENT_HEAT_CONVERTER_H
#define VG_VENTILATED_LATENT_HEAT_CONVERTER_H

#include "vapour_flux.h"

namespace vg {

class VentilatedLatentHeatConverter : public VapourFlux
{
public:
    VentilatedLatentHeatConverter(UniSim::Identifier name, QObject *parent);
    void update();
private:
    double indoorsAh, groundArea, volumeFlowRate;
    double dewRate();
    double convertedAh();
};
} //namespace


#endif
