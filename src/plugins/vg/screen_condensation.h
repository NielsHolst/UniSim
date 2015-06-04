/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREEN_CONDENSATION_H
#define VG_SCREEN_CONDENSATION_H

#include "vapour_flux_base.h"

namespace vg {

class ScreenCondensation : public VapourFluxBase
{
public:
    ScreenCondensation(UniSim::Identifier name, QObject *parent);
    void update();
private:
    double screenTemperature, indoorsTemperature, indoorsAh, screenArea;
};
} //namespace


#endif
