/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GREENHOUSE_ENERGETICS_H
#define VG_GREENHOUSE_ENERGETICS_H

#include <usbase/model.h>

namespace vg {

class GreenhouseEnergetics : public UniSim::Model
{
public:
    GreenhouseEnergetics(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double energyScreenState, KScreenEnergy, KCover, floorHec,
        Toutdoors, Tfloor, ventilation,
        shortWaveLight, longWaveLight, cropLatentHeatFlux, screenDew, coverDew, cropDew, lampHeat;

    // Variables
    double energyBalance, heatCapacity, Tunheated, dewHeat, outdoorsHeat, floorHeat;

    // Methods
    double KTotal() const;
    void updateTunheated();
};
} //namespace


#endif
