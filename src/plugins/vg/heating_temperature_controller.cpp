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
 * - _timeStepSecs_ is the integration time step [s]
 * - _greenhouseArea_ is the ground area covered by the greenhouse [m<SUP>2</SUP>]
 * - _greenhouseVolume_ is the greenhouse volume [m<SUP>s</SUP>]
 * - _indoorsTemperature_ is the ambient indoors temperature [<SUP>o</SUP>C]
 * - _heatingSetpoint_ is the setpoint at which heating is turned on [<SUP>o</SUP>C]
 * - _balanceEffect_ is the effect needed per greenhouse area to maintain the greenhouse energy balance [W/m<SUP>2</SUP>]
 * - _ventilationOngoing_ is set if ventilation is on; as a consequence _signal_ is set to the _minimumSignal_ [true/false]
 *
 * Outputs
 * ------
 * - _signal_ is the temperate of the water let into the heating pipes

 * Default dependencies
 * ------------
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 * - a _construction/geometry_ model with two ports:
 *   + _groundArea_ [m<SUP>2</SUP>]
 *   + _volumeTotal_ [m<SUP>3</SUP>]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>o</SUP>C]
 * - a _setpoints/temperature/heating_ model with a _setpoint_ port [<SUP>o</SUP>C]
 * - a _heatBalance_ model with a _value_ port [W/m<SUP>2</SUP>]
 * - a _ventilationThreshold_ child model with a _passed_ port [true/false]
 * - usually, an _effectuators/heating/pipes_ model with zero or more models of HeatPipe type
 */

HeatingTemperatureController::HeatingTemperatureController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, minimumSignal, 40.);
    Input(double, maximumSignal, 70.);
    InputRef(double, timeStepSecs, "calendar[timeStepSecs]");
    InputRef(double, greenhouseArea, "construction/geometry[groundArea]");
    InputRef(double, greenhouseVolume, "construction/geometry[volumeTotal]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, heatingSetpoint, "setpoints/temperature/heating[setpoint]");
    InputRef(double, balanceEffect, "indoors/energy[value]");
    InputRef(bool, ventilationOngoing, "./ventilationThreshold[passed]");
    Output(double, signal);
}

void HeatingTemperatureController::amend() {
    Model *pipesParent = peekOne<Model*>("effectuators/heating/pipes");
    if (pipesParent)
        pipes = pipesParent->seekChildren<HeatPipe*>("*");
    numPipes = pipes.size();
}

void HeatingTemperatureController::reset() {
    signal = minimumSignal;
}

void HeatingTemperatureController::update() {
    if (numPipes == 0) return;
//    if (ventilationOngoing) {
//        signal = minimumSignal;
//        return;
//    }
    double energyNeededPerPipe = effectNeeded()/numPipes;
    double inflowTemp = 0;
    for (auto pipe : pipes) {
        inflowTemp += pipe->temperatureNeeded(energyNeededPerPipe);
    }
    signal = minMax(minimumSignal, inflowTemp/numPipes, maximumSignal);
}

double HeatingTemperatureController::effectNeeded() {
//    double realisedTempIncrease = indoorsTemperature-outdoorsTemperature,
//           desiredTempIncrease = heatingSetpoint-outdoorsTemperature;
//    double estimate1 = realisedTempIncrease>0. ?
//           desiredTempIncrease/realisedTempIncrease*balanceEffect : 0.;

    double tempIncrease = heatingSetpoint - indoorsTemperature;
    double extraEnergy = tempIncrease > 0. ?
                         tempIncrease*CpAir*greenhouseVolume*RhoAir :
                         0.;
    // J = K * J/kg/K * m3 * kg/m3
    double balanceEnergy = balanceEffect < 0. ?
                           -balanceEffect*timeStepSecs*greenhouseArea :
                           0.;
    // J = W/m2 * s * m2
    return extraEnergy + balanceEnergy;
//    double estimate2 = extraEnergy + balanceEnergy;
//    return max(estimate1, estimate2);
}


} //namespace

