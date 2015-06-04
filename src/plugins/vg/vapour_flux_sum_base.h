/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VAPOUR_FLUX_SUM_BASE_H
#define VG_VAPOUR_FLUX_SUM_BASE_H

#include "vapour_flux_base.h"

namespace vg {

class VapourFluxSumBase : public VapourFluxBase
{
public:
    VapourFluxSumBase(UniSim::Identifier name, QObject *parent);
    void initialize();
    void update();
private:
    // Data
    struct Ptr {
        const double *conductance, *vapourFlux, *gain;
    };
    QVector<Ptr> ptrs;
    // Methods
    virtual QList<VapourFluxBase*> fluxes() = 0;
};
} //namespace


#endif
