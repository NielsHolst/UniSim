/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_ENERGY_BALANCE_H
#define VG_ENERGY_BALANCE_H

#include <QVector>
#include <usbase/model.h>

namespace vg {

class EnergyBalance : public UniSim::Model
{
public:
    EnergyBalance(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    double irradiation, heatPipes, growthLight, value, potentialVentilation;
    QVector<const double *> otherFluxes;
    UniSim::Model *realisedVentilationModel;

    void setVentilation();
};
} //namespace


#endif
