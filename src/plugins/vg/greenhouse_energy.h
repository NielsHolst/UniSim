/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GREENHOUSE_ENERGY_H
#define VG_GREENHOUSE_ENEGY_H

#include <usbase/model.h>

namespace vg {

class GreenhouseEnergy : public UniSim::Model
{
public:
    GreenhouseEnergy(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double spScreenEnergy, KScreenEnergy, KCover, floorHec,
        Toutdoors, Tindoors, ventilation, stdVentilation,
        rnLight, bnLight, LEtot, dewCondensation, lampHeat,
        spHeating;

    // Variables
    double energyBalance, heatCapacity, heatingDemand, Tunheated;
};
} //namespace


#endif
