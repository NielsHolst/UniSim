/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BASE_GROWTH_LIGHT_H
#define VG_BASE_GROWTH_LIGHT_H

#include <usbase/model.h>

namespace vg {

class BaseGrowthLight : public UniSim::Model
{
public:
    BaseGrowthLight(UniSim::Identifier name, QObject *parent);
protected:
    double heatEmission, longWaveEmission, shortWaveEmission, parEmission, energyUse;
    bool currentlyOn;
    void noLight();
};
} //namespace


#endif
