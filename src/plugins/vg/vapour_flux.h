/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VAPOUR_FLUX_H
#define VG_VAPOUR_FLUX_H

#include <usbase/model.h>

namespace vg {

class VapourFlux : public UniSim::Model
{
public:
    VapourFlux(UniSim::Identifier name, QObject *parent);
    void reset();
protected:
    double timeStep,
        conductance, gain, vapourFlux, dewDensity, dewThickness;
};
} //namespace


#endif
