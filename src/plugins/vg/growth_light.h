/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GROWTH_LIGHT_H
#define VG_GROWTH_LIGHT_H

#include "base_growth_light.h"

namespace vg {

class GrowthLight : public BaseGrowthLight
{
public:
    GrowthLight(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    bool on;
    double heatEmissionOn, longWaveEmissionOn, shortWaveEmissionOn, parEmissionOn, energyUseOn,
        age, lifeTime, minPeriod, timeStep, r, currentPeriod, totalPeriod;
};
} //namespace


#endif
