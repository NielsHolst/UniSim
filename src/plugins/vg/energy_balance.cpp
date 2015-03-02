/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_balance.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyBalance)

/*! \class EnergyBalance
 * \brief The balance of all energy fluxes
 *
 * Inputs
 * ------
 * - _irradiation_ is the heat gained through sun irradiation per greenhouse area [W/m<SUP>2</SUP>]
 * - _heatPipes_ is the heat emitted by the heating pipes [W/m<SUP>2</SUP>]
 * - _growhtLight_ is the long wave emission (thermal radiation, Bn) [W/m<SUP>2</SUP>]
 *
 * Output
 * ------
 * - _value_ is the energy balance per greenhouse area [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - an _indoors/radiation_ model with a total port [W/m<SUP>2</SUP>]
 * - an _actuators/heating/pipes_ model with an _effect_ port [W/m<SUP>2</SUP>]
 * - an _actuators/growthLight_ model with a _longWaveEmission_ port [W/m<SUP>2</SUP>]
 *
 * Optional dependencies
 * ------------
 * - some (0..n) child models with a _flux_ port [W/m<SUP>2</SUP>]
 */

EnergyBalance::EnergyBalance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, irradiation, "indoors/radiation[total]");
    InputRef(double, heatPipes, "actuators/heating/pipes[effect]");
    InputRef(double, growthLight, "actuators/growthLights[longWaveEmission]");
//    InputRef(double, potentialVentilation, "./ventilation/potential[flux]");
    Output(double, value);
}

void EnergyBalance::initialize() {
    otherFluxes.clear();
    auto models = seekChildren<Model*>("*");
    for (auto model : models) {
        auto flux = model->peekValuePtr<double>("flux");
        if (flux)
            otherFluxes << flux;
    }
//    auto realisedVentilation = peekOneChild<Model*>("ventilation/realised");
//    if (!realisedVentilation)
//        throw Exception("EnergyBalance model must have a child model called: 'ventilation/realised'", this);
}

void EnergyBalance::reset() {
    value = 0.;
}

void EnergyBalance::update() {
    value = irradiation + heatPipes + growthLight;
    for (auto flux : otherFluxes)
        value += *flux;
    setVentilation();
}
void EnergyBalance::setVentilation() {
//    Q_ASSERT(false);
    /* Unfinished:
    double realisedVentilation;
    if (sign(value) == sign(potentialVentilation))
        realisedVentilation = potentialVentilation;
    else {
        double demand = fabs(value),
               available
        realisedVentilation = demand*(1-exp(-potential/demand));

    }
    */

}


} //namespace

