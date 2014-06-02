/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_FLUX_CONDENSATION_H
#define VG_ENERGY_FLUX_CONDENSATION_H

#include <QVector>
#include <usbase/model.h>

namespace vg {

class EnergyFluxCondensation : public UniSim::Model
{
public:
    EnergyFluxCondensation(UniSim::Identifier name, QObject *parent);
    void reset();
    void initialize();
    void update();

private:
    double flux;
    QVector<const double*> fluxes;
};
} //namespace


#endif
