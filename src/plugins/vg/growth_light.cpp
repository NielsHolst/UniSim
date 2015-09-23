/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "growth_light.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

UniSim::StringMap<GrowthLight::Type> GrowthLight::types;

PUBLISH(GrowthLight)

/*! \class GrowthLight
 * \brief Current growth light
 *
 * Inputs
 * ------
 * - _lampType_ is the type of growth light[HPSL, LED]
 * - _intensity_ is the power of installed lamps per greenhouse area [W/m<SUP>2</SUP>]
 * - _ballastCorrection_ is set to >1 (e.g, 1.15) if capacity includes ballast (>=1)
 * - _age_ is the age of the lamps at simulation start [h]
 * - _lifeTime_ is the age at which light output is reduced to 90% [h]
 * - _timeStep_ is the integration time step [s]
 *
 * Outputs
 * ------
 * - _currentPeriod_ duration of the current period with light on [min]
 * - _totalPeriod_ total period with light on [h]
 *
 * Default dependencies
 * ------------
 * - a _controllers/growthLight model with a _signal_ port [true,false]
 * - a _construction/growthLight model with the eight ports:
 * + _heatEmission_ [W/m<SUP>2</SUP>]
 * + _longWaveEmission_ [W/m<SUP>2</SUP>]
 * + _shortWaveEmission_ [W/m<SUP>2</SUP>]
 * + _parEmission_ [W/m<SUP>2</SUP>]
 * + _energyUse_ is the energy used when light is on [W/m<SUP>2</SUP>]
 * + _minPeriod_ [min]
 * + _age_ [h]
 * + _lifeTime_  [h]
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 */

GrowthLight::GrowthLight(Identifier name, QObject *parent)
    : GrowthLightBase(name, parent)
{
    Input2(QString, lampTypeStr, type, "HPSL");
    Input(double, intensity, 40.);
    Input(double, ballastCorrection, 1.);
    Input(double, age, 0.);
    Input(double, lifeTime, 12000.);
    InputRef(bool, on, "controllers/growthLight[signal]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");

    Output(double, currentPeriod);
    Output(double, totalPeriod);

    types["hpsl"] = Hpsl;
    types["led"] = Led;
}

void GrowthLight::reset() {
    switch (types.seek(lampTypeStr.toLower(), this)) {
    case Hpsl:
        attributes.heatCoef = 0.23;
        attributes.longWaveCoef = 0.42;
        attributes.shortWaveCoef = 0.50;
        attributes.parCoef = 0.31;
        attributes.minPeriodOn = 30.;
        break;
    case Led:
        attributes.heatCoef = 0.02;
        attributes.longWaveCoef = 0.05;
        attributes.shortWaveCoef = 0.82;
        attributes.parCoef = 0.82;
        attributes.minPeriodOn = 0.;
        break;
    }

    double netCapacity = intensity/ballastCorrection;
    netAttributes.heatEmission = attributes.heatCoef*netCapacity;
    netAttributes.longWaveEmission = attributes.longWaveCoef*netCapacity;
    netAttributes.shortWaveEmission = attributes.shortWaveCoef*netCapacity;
    netAttributes.parEmission = attributes.parCoef*netCapacity;
    netAttributes.energyFlux = ballastCorrection*intensity;
    degradationRate = log(0.9)/lifeTime;
    currentPeriod = 0.;
    totalPeriod = age;
    noLight();
}

void GrowthLight::update() {
    currentlyOn = on ||
                  ( currentlyOn && (currentPeriod < attributes.minPeriodOn) );
    if (currentlyOn) {
        currentPeriod += timeStep/60.;
        totalPeriod += timeStep/3600.;
        double f = exp(degradationRate*(age+totalPeriod));
        heatEmission = netAttributes.heatEmission + (1-f)*(netAttributes.longWaveEmission+netAttributes.shortWaveEmission);
        longWaveEmission = f*netAttributes.longWaveEmission;
        shortWaveEmission = f*netAttributes.shortWaveEmission;
        totalEmission = heatEmission + longWaveEmission + shortWaveEmission;
        parEmission = f*netAttributes.parEmission;
        energyFlux = netAttributes.energyFlux;
    }
    else
        noLight();
}

} //namespace

