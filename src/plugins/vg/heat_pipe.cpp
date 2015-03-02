/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "general.h"
#include "heat_pipe.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
PUBLISH(HeatPipe)

/*! \class HeatPipe
 * \brief Computes heat pipe temperature and emittance
 *
 * Inputs
 * ------
 * - _length_ is pipe length per greenhouse area [m/m<SUP>2</SUP>]. If zero then it is calculated as _totalLength_/_greenhouseArea.
 * - _diameter_ is the pipe inner diameter [mm]
 * - _flowRate_ is the flow rate [m<SUP>3</SUP>/h]
 * - _inflowTemperature_ is the temperature of the water flowing into the pipe [<SUP>o</SUP>C]
 * - _indoorsTemperature_ is the ambient temperature of the greenhouse [<SUP>o</SUP>C]
 * - _timeStep_ is the integration time step [s]
 * - _greenhouseArea_ is the ground area covered by the greenhouse [m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _temperature_ is the pipe temperature [<SUP>o</SUP>C]
 * - _effect_ is the energy emission per greenhouse area [W/m<SUP>2</SUP>]

 * Default dependencies
 * ------------
 * - a _controllers/heating/temperature_ model wit a _signal_ port [<SUP>o</SUP>C]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>o</SUP>C]
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 * - a _construction/geometry_ model with a _groundArea_ port [m<SUP>2</SUP>]
 */

HeatPipe::HeatPipe(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, length, 1.);
    Input(double, diameter, 51.);
    Input(double, flowRate, 20.);
    InputRef(double, inflowTemperature, "controllers/heating/temperature[signal]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, greenhouseArea, "construction/geometry[groundArea]");
    Output(double, temperature);
    Output(double, effect);
}

void HeatPipe::initialize() {
    double x = minMax(26., diameter, 51.);
    U = 0.0131*x + 0.105;       // W/m2/K per m pipe
}

void HeatPipe::reset() {
    if (diameter <= 0.) {
        QString msg {"Pipe diameter (%1) must be > 0 mm"};
        throw Exception(msg.arg(diameter), this);
    }
    temperature = indoorsTemperature;
    double r = diameter/2.;
    volume = length*PI*r*r*10./100/100; // L/m2
    updateFlow();
    selfTest();
    Q_ASSERT(volume>0.);
}

void HeatPipe::update() {
    updateFlow();
    Effect inflow = specificEffect(inflowTemperature),
           stagnant = specificEffect(temperature);
    effect = inflow.effect*propFlow + stagnant.effect*(1-propFlow);
    temperature = inflow.temperature*propFlow + stagnant.temperature*(1-propFlow);
    // K = W/m2 * s / J/kg/K / kg/m2
//    double tempLoss = effect*timeStep/CpWater/volume;
//    temperature = propFlow*inflowTemperature + (1-propFlow)*temperature - tempLoss;
}

void HeatPipe::updateFlow() {
    double flowVolume = flowRate*timeStep*1000./3600,       // L = m3/h * s * L/m3 / s/h
           totalVolume = volume*greenhouseArea;             // L = L/m2 * m2
    propFlow = min(flowVolume/totalVolume, 1.);
}

//! Compute effect of pipe [W/m] given a certain pipe temperature [<SUP>o</SUP>C]
HeatPipe::Effect HeatPipe::specificEffect(double Tpipe) {
//    double Tdiff = Tpipe - indoorsTemperature;
//    return (Tdiff < 0.) ? 0. : pow(Tdiff, exponent)*slope;
    if (indoorsTemperature > Tpipe)
        return Effect{0,0};
    double Cwater = volume*CpWater, // J/m2/K =  kg/m2 * J/kg/K
           rate = U*length/Cwater, // s-1
           TpipeNew = propExpIntegral(Tpipe, indoorsTemperature, rate, timeStep, exponent),
           effect = Cwater*(Tpipe-TpipeNew)/timeStep; // W/m2 = J/m2/K * K / s
    return Effect{effect, TpipeNew};
}

//! Compute pipe temperature [<SUP>o</SUP>C] needed to yield a certain effect [W/m]
double HeatPipe::pipeTemperature(double specificEffect) {
    double Tdiff = pow(specificEffect/U, 1./exponent);
    return indoorsTemperature + Tdiff;
//    double Cwater = volume*CpWater, // J/m2/K =  kg/m2 * J/kg/K
//           rate = U*length/Cwater, // s-1
//           TpipeNew = temperature - specificEffect*timeStep/Cwater,
//           Tpipe = invPropExpIntegral(TpipeNew, indoorsTemperature, rate, timeStep, exponent);
//    return Tpipe;
}

//! Compute inflow temperature [<SUP>o</SUP>C] needed to yield a certain effect in the greenhouse [W/m<SUP>2</SUP>]
double HeatPipe::inflowTemperatureNeeded(double effect) {
    // Effect needed per pipe length
    double pipeEffectNeeded = effect/length;                            // W/m
    // Effect provided by proportion of stagnant water
    double stagnantEffect = specificEffect(temperature).effect*(1-propFlow);   // W/m
    // Effect needed from proportion of incoming water
    double neededInflowEffect = (pipeEffectNeeded - stagnantEffect)/max(propFlow, 1e-6);    // W/m
    //
    return (neededInflowEffect > 0) ? pipeTemperature(neededInflowEffect) : 0.;
}

void HeatPipe::selfTest() {
//    const double Tpipe1 = 80.;
//    double savePropFlow = propFlow;
//    propFlow = 0.05;
//    double effect1 = specificEffect(Tpipe1).effect;
//    double Tpipe2 = pipeTemperature(effect1);
//    if (TestNum::ne(Tpipe1, Tpipe2)) {
//        QString msg = "Self-test failed, Tpipe1=%1 differs from Tpipe2=%2. Effect=%3.";
//        throw Exception(msg.arg(Tpipe1).arg(Tpipe2).arg(effect1), this);
//    }
//    propFlow = savePropFlow;
}

} //namespace

