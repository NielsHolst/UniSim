/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_HEAT_PIPES_H
#define VG_HEAT_PIPES_H

#include <QVector>
#include <usbase/model.h>

namespace vg {

class HeatPipes : public UniSim::Model
{
public:
    HeatPipes(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Outputs
    double maxEnergyFlux;
    // Data
    QVector<const double*> maxEnergyFluxes;
};
} //namespace


#endif
