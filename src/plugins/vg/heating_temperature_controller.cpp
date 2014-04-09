/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "construction_cover.h"
#include "heat_pipe.h"
#include "heating_temperature_controller.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
HeatingTemperatureController::HeatingTemperatureController(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(minimumSignal), 40., "Minimum temperature in water inlet (oC)");
    addParameter<double>(Name(maximumSignal), 70., "Maximum temperature in water inlet (oC)");
    addParameterRef<double>(Name(heatingFlow), "../flow[signal]");
    addParameterRef<double>(Name(timeStepSecs), "calendar[timeStepSecs]");
    addParameterRef<double>(Name(greenhouseArea), "construction/geometry[groundArea]");
    addParameterRef<double>(Name(greenhouseVolume), "construction/geometry[volumeTotal]");
    addParameterRef<double>(Name(indoorsTemperature), "indoors/temperature[value]");
    addParameterRef<double>(Name(heatingSetpoint), "setpoints/temperature/heating[setpoint]");
    addVariable<double>(Name(signal), "Temperature in water inlet (oC)");
}

void HeatingTemperatureController::amend() {
    covers = seekMany<ConstructionCover*>("construction/cover/*");
    Model *pipesParent = peekOne<Model*>("effectuators/pipes");
    if (pipesParent)
        pipes = pipesParent->seekChildren<HeatPipe*>("*");
    numPipes = pipes.size();
}

void HeatingTemperatureController::reset() {
    signal = (minimumSignal + maximumSignal)/2.;
    coverK = calcCoverK();
}

void HeatingTemperatureController::update() {
    const double energyLoss = 10000;
    if (numPipes > 0) {
        double energyLossPerPipe = energyLoss/numPipes;
        double inflowTemp = 0;
        for (auto pipe : pipes) {
            inflowTemp += pipe->temperatureNeeded(energyLossPerPipe);
        }
        signal = minMax(minimumSignal, inflowTemp/numPipes, maximumSignal);
    }
}

double HeatingTemperatureController::calcCoverK() {
    double sumK(0), sumArea(0);
    for (auto cover : covers) {
        sumK += cover->pullValue<double>("K");
        sumArea += cover->pullValue<double>("area");
    }
    return (sumArea > 0) ? sumK/sumArea : 0;
}



} //namespace

