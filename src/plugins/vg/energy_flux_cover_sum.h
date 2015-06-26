/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_COVER_SUM_H
#define VG_ENERGY_FLUX_COVER_SUM_H

#include "energy_flux_base.h"

namespace vg {

class EnergyFluxCoverSum : public EnergyFluxBase
{
public:
    EnergyFluxCoverSum(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Input
    QString toAdd;
    double greenhouseArea;
    // Data
    struct CoverInfo {
        const double *flux, *area;
        double totalFlux() const { return *area*(*flux); }
    };
    QVector<CoverInfo> coverInfos;
    // Methods
    void initAreas();
    void initFluxes();
};
} //namespace


#endif
