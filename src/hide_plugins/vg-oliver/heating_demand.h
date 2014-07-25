/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_HEATING_DEMAND_H
#define VG_HEATING_DEMAND_H

#include <usbase/model.h>

namespace vg {

class HeatingDemand : public UniSim::Model
{
public:
    HeatingDemand(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Parameters
    double Tminimum, energyBalance, heatCapacity;

    // Variables
    double total, perPipe;
    int numPipes;
};
} //namespace


#endif
