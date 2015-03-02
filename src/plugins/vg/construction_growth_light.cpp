/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "construction_growth_light.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

UniSim::StringMap<ConstructionGrowthLight::Type> ConstructionGrowthLight::types;

PUBLISH(ConstructionGrowthLight)

/*! \class ConstructionGrowthLight
 * \brief Specification of growth light
 *
 * Inputs
 * ------
 * - _lampType_ is the type of growth light[HPSL, LED]
 * - _capacity_ is the capacity of installed lamps [W/m<SUP>2</SUP>]
 * - _ballastCorrection_ is set to >1 (e.g, 1.15) if capacity includes ballast (>=1)
 * - _age_ is the age of the lamps at simulation start [h]
 * - _lifeTime_ is the age at which light output is reduced to 90% [h]
 * - _minPeriod_ is the minimum period the lamp must remain switched on [min]
 *
 * Output
 * ------
 *   _heatEmission_ is the convective heat emission [W/m<SUP>2</SUP>]
 *   _longWaveEmission_ is the long wave emission (thermal radiation, Bn) [W/m<SUP>2</SUP>]
 *   _shortWaveEmission_ is the short wave emission (visible light, Rn) [W/m<SUP>2</SUP>]
 *   _parEmission_ is the PAR emision [W/m<SUP>2</SUP>]
 *   _energyUse_ is the energy used [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - none
 */

ConstructionGrowthLight::ConstructionGrowthLight(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input2(QString, lampTypeStr, type, "HPSL");
    Input(double, capacity, 40.);
    Input(double, ballastCorrection, 1.);
    Input(double, age, 0.);
    Input(double, lifeTime, 12000.);
    Input(double, minPeriod, 0.);

    Output(double, heatEmission);
    Output(double, longWaveEmission);
    Output(double, shortWaveEmission);
    Output(double, parEmission);
    Output(double, energyUse);

    types["hpsl"] = Hpsl;
    types["led"] = Led;
}

void ConstructionGrowthLight::initialize() {

    switch (types.seek(lampTypeStr.toLower(), this)) {
    case Hpsl:
        heatCoef = 0.23;
        longWaveCoef = 0.42;
        shortWaveCoef = 0.50;
        parCoef = 0.31;
        minPeriod = 30.;
        break;

    case Led:
        heatCoef = 0.02;
        longWaveCoef = 0.05;
        shortWaveCoef = 0.82;
        parCoef = 0.82;
        minPeriod = 0.;
        break;
    }
}

void ConstructionGrowthLight::reset() {
    double netCapacity = capacity/ballastCorrection;
    heatEmission = heatCoef*netCapacity;
    longWaveEmission = longWaveCoef*netCapacity;
    shortWaveEmission = shortWaveCoef*netCapacity;
    parEmission = parCoef*netCapacity;
    energyUse = ballastCorrection*capacity;
}

} //namespace

