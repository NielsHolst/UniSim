/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_COVER_CONDENSATION_H
#define VG_COVER_CONDENSATION_H

#include "vapour_flux.h"

namespace vg {

class CoverCondensation : public VapourFlux
{
public:
    CoverCondensation(UniSim::Identifier name, QObject *parent);
    void update();
private:
    double coverTemperature, indoorsTemperature, indoorsAh, coverArea, groundArea;
};
} //namespace


#endif
