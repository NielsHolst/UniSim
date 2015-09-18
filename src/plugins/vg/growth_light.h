/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_GROWTH_LIGHT_H
#define VG_GROWTH_LIGHT_H

#include <usbase/string_map.h>
#include "growth_light_base.h"

namespace vg {

class GrowthLight : public GrowthLightBase {
public:
    GrowthLight(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    QString lampTypeStr;
    bool on;
    double timeStep, intensity, ballastCorrection, age, lifeTime;

    // Outputs
    double currentPeriod, totalPeriod;

    // Lamp attributes
    enum Type {Hpsl, Led};
    static UniSim::StringMap<Type> types;
    struct { double heatCoef, longWaveCoef, shortWaveCoef, parCoef, minPeriodOn; } attributes;
    struct { double heatEmission, longWaveEmission, shortWaveEmission, parEmission, energyFlux; } netAttributes;
    double degradationRate;
};
} //namespace


#endif
