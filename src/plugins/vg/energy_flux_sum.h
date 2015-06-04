/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_SUM_H
#define VG_ENERGY_FLUX_SUM_H

#include <QVector>
#include "energy_flux_base.h"

namespace vg {

class EnergyFluxSum : public EnergyFluxBase
{
public:
    EnergyFluxSum(UniSim::Identifier name, QObject *parent);
    void initialize();
    void update();

private:
    QVector<const double *> fluxes;
};
} //namespace


#endif
