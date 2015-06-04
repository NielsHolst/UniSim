/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VAPOUR_FLUX_BASE_H
#define VG_VAPOUR_FLUX_BASE_H

#include <usbase/model.h>

namespace vg {

class VapourFluxBase : public UniSim::Model
{
public:
    VapourFluxBase(UniSim::Identifier name, QObject *parent);
    void reset();
protected:
    // Outputs
    double conductance, gain, vapourFlux;
};
} //namespace


#endif
