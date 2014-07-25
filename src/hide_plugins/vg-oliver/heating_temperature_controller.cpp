/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "cover.h"
#include "general.h"
#include "heat_pipe.h"
#include "heating_temperature_controller.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
PUBLISH(HeatingTemperatureController)

/*! \class HeatingTemperatureController
 * \brief Controls the temperate of the water let into the heating pipes
 *
 * Inputs
 * ------
 * - _minimumSignal_ is the minimum temperature of the water inlet [<SUP>o</SUP>C]
 * - _maximumSignal_ is the maximum temperature of the water inlet [<SUP>o</SUP>C]
 * - _timeStep_ is the integration time step [s]
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _indoorsTemperature_ is the ambient indoors temperature [<SUP>o</SUP>C]
 * - _heatingSetpoint_ is the setpoint at which heating is turned on [<SUP>o</SUP>C]
 * - _balanceEffect_ is the effect needed per greenhouse area to maintain the greenhouse energy balance [W/m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _signal_ is the temperate of the water let into the heating pipes

 * Default dependencies
 * ------------
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 * - a _construction/geometry_ model with an _averageHeight_ port [m]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>o</SUP>C]
 * - a _setpoints/temperature/heating_ model with a _setpoint_ port [<SUP>o</SUP>C]
 * - a _heatBalance_ model with a _value_ port [W/m<SUP>2</SUP>]
 * - usually, an _actuators/heating/pipes_ model with zero or more models of HeatPipe type
 */

HeatingTemperatureController::HeatingTemperatureController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, minimumSignal, 40.);
    Input(double, maximumSignal, 70.);
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, averageHeight, "construction/geometry[averageHeight]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, heatingSetpoint, "setpoints/temperature/heating[setpoint]");
    InputRef(double, balanceEffect, "indoors/energy[value]");
    Output(double, signal);
}

void HeatingTemperatureController::amend() {
    Model *pipesParent = peekOne<Model*>("actuators/heating/pipes");
    if (pipesParent)
        pipes = pipesParent->seekChildren<HeatPipe*>("*");
    numPipes = pipes.size();
}

void HeatingTemperatureController::reset() {
    signal = minimumSignal;
}

void HeatingTemperatureController::update() {
    if (numPipes == 0) return;
    double energyNeededPerPipe = effectNeeded()/numPipes;
    double inflowTemp = 0;
    for (auto pipe : pipes) {
        inflowTemp += pipe->inflowTemperatureNeeded(energyNeededPerPipe);
    }
    signal = minMax(minimumSignal, inflowTemp/numPipes, maximumSignal);
}

/*!
 * \brief HeatingTemperatureController::effectNeeded
 * \return Energy needed for the whole greenhouse in this time step [W/<SUP>2</SUP>]
 */
double HeatingTemperatureController::effectNeeded() {
    double tempIncrease = heatingSetpoint - indoorsTemperature;
    // W/m2 = K * J/kg/K * m3/m2 * kg/m3 / s
    double tempEffect = tempIncrease*CpAir*averageHeight*RhoAir/timeStep;
    double demand = max(tempEffect,0.) - min(balanceEffect,0.);
    demand = tempEffect - balanceEffect;
    for (auto pipe : pipes) {
        demand += pipe->pullValue<double>("effect");
    }
    return max(demand, 0.);
}


} //namespace

