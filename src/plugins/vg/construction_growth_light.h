/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CONSTRUCTION_GROWTH_LIGHT_H
#define VG_CONSTRUCTION_GROWTH_LIGHT_H

#include <usbase/model.h>
#include <usbase/string_map.h>

namespace vg {

class ConstructionGrowthLight : public UniSim::Model
{
public:
    ConstructionGrowthLight(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
private:
    // Inputs
    QString lampTypeStr;
    double capacity, ballastCorrection, age, lifeTime, minPeriod;

    // Outputs
    double heatEmission, longWaveEmission, shortWaveEmission, parEmission, energyUse;

    // Lamp attributes
    enum Type {Hpsl, Led};
    static UniSim::StringMap<Type> types;
    double heatCoef, longWaveCoef, shortWaveCoef, parCoef;
};
} //namespace


#endif
