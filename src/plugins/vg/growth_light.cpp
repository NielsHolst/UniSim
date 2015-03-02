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

PUBLISH(GrowthLight)

/*! \class GrowthLight
 * \brief Current growth light
 *
 * Inputs
 * ------
 * - _on_ tells whether light should be on[true,false]
 * - _heatEmissionOn_ is the convective heat emission when light is on [W/m<SUP>2</SUP>]
 * - _longWaveEmissionOn_ is the long wave emission (thermal radiation, Bn) when light is on [W/m<SUP>2</SUP>]
 * - _shortWaveEmissionOn_ is the short wave emission (visible light, Rn) when light is on [W/m<SUP>2</SUP>]
 * - _parEmissionOn_ is the PAR emision when light is on [W/m<SUP>2</SUP>]
 * - _energyUseOn_ is the energy used when light is on [W/m<SUP>2</SUP>]
 * - _minPeriod_ is the minimum period the lamp must remain switched on [min]
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
    : BaseGrowthLight(name, parent)
{
    InputRef(bool, on, "controllers/growthLight[signal]");
    InputRef(double, heatEmissionOn, "construction/growthLight[heatEmission]");
    InputRef(double, longWaveEmissionOn, "construction/growthLight[longWaveEmission]");
    InputRef(double, shortWaveEmissionOn, "construction/growthLight[shortWaveEmission]");
    InputRef(double, parEmissionOn, "construction/growthLight[parEmission]");
    InputRef(double, energyUseOn, "construction/growthLight[energyUse]");
    InputRef(double, age, "construction/growthLight[age]");
    InputRef(double, lifeTime, "construction/growthLight[lifeTime]");
    InputRef(double, minPeriod, "construction/growthLight[minPeriod]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    Output(double, currentPeriod);
    Output(double, totalPeriod);
}

void GrowthLight::reset() {
    noLight();
    r = log(0.9)/lifeTime;
    currentPeriod = totalPeriod = 0.;
}

void GrowthLight::update() {
    bool keepOn = currentlyOn && currentPeriod < minPeriod;
    if (on || keepOn) {
        currentPeriod += timeStep/60.;
        totalPeriod += timeStep/3600.;
        double f = exp(r*(age+totalPeriod));
        heatEmission = heatEmissionOn + (1-f)*(longWaveEmissionOn+shortWaveEmissionOn);
        longWaveEmission = f*longWaveEmissionOn;
        shortWaveEmission = f*shortWaveEmissionOn;
        parEmission = f*parEmissionOn;
        energyUse = energyUseOn;
        currentlyOn = true;
    }
    else
        noLight();
}

} //namespace

