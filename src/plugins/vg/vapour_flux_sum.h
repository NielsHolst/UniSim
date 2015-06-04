/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_VAPOUR_FLUX_SUM_H
#define VG_VAPOUR_FLUX_SUM_H

#include "vapour_flux_sum_base.h"

namespace vg {

class VapourFluxSum : public VapourFluxSumBase
{
public:
    VapourFluxSum(UniSim::Identifier name, QObject *parent);
private:
    // Input
    QString toAdd;
    // Methods
    QList<VapourFluxBase*> fluxes();
    QList<VapourFluxBase*> childFluxes();
    QList<VapourFluxBase*> referredFluxes();
};
} //namespace


#endif
