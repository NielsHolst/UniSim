/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "energy_flux_heating_supply.h"
#include "heat_pipe.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxHeatingSupply)

/*! \class EnergyFluxHeatingSupply
 * \brief Flux of energy need to warm up to heating setpoint
 * Inputs
 * ------
 * - _maxHeating_ is the maximum heating that can be supplied [W/m<SUP>2</SUP>]
 * - _demand_ is the heating needed [W/m<SUP>2</SUP>]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _value_ is the heating supplied [W/m<SUP>2</SUP>]
 * - _energyUsed_ is the accumuated energy used [MWh/m<SUP>2</SUP>]
 */

EnergyFluxHeatingSupply::EnergyFluxHeatingSupply(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(double, maxHeating, 10000.);
    InputRef(double, demand, "heating/demand[value]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    Output(double, energyUsed);
}

void EnergyFluxHeatingSupply::reset() {
    EnergyFluxBase::reset();
    energyUsed = 0.;
}

void EnergyFluxHeatingSupply::update() {
    value = min(demand, maxHeating);
    energyUsed += value*timeStep/3600./1e6;
}


//void EnergyFluxHeatingSupply::initialize() {
//    pipes = seekMany<HeatPipe*>("actuators/heating/pipes/*");
//}

//void EnergyFluxHeatingSupply::setHeatPipes() {
//    double energyFluxSum{0};
//    // Set all pipes at minimum temperature
//    for (auto pipe : pipes) {
//        double pipeEnergyFluxMin = pipe->calcEnergyFluxMin();
//        energyFluxSum += pipeEnergyFluxMin;
//        pipe->pushValue<double>("energyFlux", pipeEnergyFluxMin);
//    }
//    // Turn on one pipe at a time until demand is satisfied
//    bool satisfied = (energyFluxSum >= value);
//    for (auto pipe : pipes) {
//        if (satisfied) break;
//        energyFluxSum -= pipe->calcEnergyFluxMin();
//        double pipeEnergyFluxMax = pipe->calcEnergyFluxMax();
//        satisfied = (energyFluxSum + pipeEnergyFluxMax >= value);
//        double pipeEnergyFlux = satisfied ? value - energyFluxSum : pipeEnergyFluxMax;
//        pipe->pushValue<double>("energyFlux", pipeEnergyFlux);
//    }
//}

} //namespace

