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
 * - _length_ is the pipe length [m]
 * - _diameter_ is the pipe inner diameter [mm]
 * - _flow_ is the flow rate [m<SUP>3</SUP>/h]
 * - _inflowTemperature_ is the temperature of the water flowing into the pipe [<SUP>o</SUP>C]
 * - _indoorsTemperature_ is the ambient temperature of the greenhouse [<SUP>o</SUP>C]
 * - _greenhouseArea_ is the ground area occupied by the greenhouse [m<SUP>2</SUP>]
 * - _timeStepSecs_ is the length of the integration time step [s]
 *
 * Outputs
 * ------
 * - _temperature_ is the pipe temperature [<SUP>o</SUP>C]
 * - _effect_ is the energy emission per greenhouse area [W/m<SUP>2</SUP>]

 * Default dependencies
 * ------------
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 * - the nearest _flow_ model with a _state_ port [true/false]
 * - a _shunt_ child model with a _state_ port [<SUP>o</SUP>C]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>o</SUP>C]
 */

HeatPipe::HeatPipe(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, length, 200.);
    Input(double, diameter, 51.);
    InputRef(double, flowRate, ".../flow[state]");
    InputRef(double, inflowTemperature, "./shunt[state]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, greenhouseArea, "construction/geometry[groundArea]");
    InputRef(double, timeStepSecs, "calendar[timeStepSecs]");
    Output(double, temperature);
    Output(double, effect);
}

void HeatPipe::initialize() {
    double x = minMax(26., diameter, 51.);
    slope = 0.0131*x + 0.105;
}

void HeatPipe::reset() {
    if (length <= 0.) {
        QString msg {"Pipe length (%1) must be > 0 m"};
        throw Exception(msg.arg(length), this);
    }
    if (diameter <= 0.) {
        QString msg {"Pipe diameter (%1) must be > 0 mm"};
        throw Exception(msg.arg(diameter), this);
    }
    temperature = indoorsTemperature;
    double r = diameter/2.;
    volume = length*PI*r*r*10./100/100; // liter
    double flowVolume = flowRate*timeStepSecs*1000./3600;  // liter
    propFlow = min(flowVolume/volume,1.);
    selfTest();
}

void HeatPipe::update() {
//    double avgTemp = propFlow*inflowTemperature + (1 - propFlow)*temperature,
//           totalEffect = specificEffect(avgTemp)*length,
//           energyEmitted = totalEffect*timeStepSecs;
//    effect = totalEffect/greenhouseArea;
//    temperature = avgTemp - energyEmitted/CpWater/volume;
    double
        inflowEffect = specificEffect(inflowTemperature)*propFlow*length,
        stagnantEffect = specificEffect(temperature)*(1-propFlow)*length,
        totalEffect = inflowEffect + stagnantEffect;
    effect = totalEffect/greenhouseArea;

    double
        inflowEmitted = inflowEffect*timeStepSecs,
        inflowTempLoss = inflowEmitted/CpWater/(volume*propFlow),
        finalInflowTemp = inflowTemperature - inflowTempLoss,
        stagnantEmitted = stagnantEffect*timeStepSecs,
        stagnantTempLoss = stagnantEmitted/CpWater/(volume*(1-propFlow)),
        finalStagnantTemp = temperature - stagnantTempLoss;
    temperature = propFlow*finalInflowTemp + (1-propFlow)*finalStagnantTemp;
    if (temperature<0)
        return;
}

//! Compute effect of pipe (W/m), given the pipe temperature _Tpipe_ (oC)
double HeatPipe::specificEffect(double Tpipe) {
    double Tdiff = Tpipe - indoorsTemperature;
    return (Tdiff < 0.) ? 0. : pow(Tdiff, exponent)*slope;
}

//! Compute pipe temperature (oC) needed to emit the given amount of _energy_ (J)
double HeatPipe::temperatureNeeded(double energy) {
    double Tdiff = 0;
    if (energy > 0.) {
        double effect = energy/(length*propFlow)/timeStepSecs;
        Tdiff = pow(effect/slope, 1./exponent);
    }
    return indoorsTemperature + Tdiff;
}

void HeatPipe::selfTest() {
    const double Tpipe1 = 80.;
    double savePropFlow = propFlow;
    propFlow = 0.05;
    double effect1 = specificEffect(Tpipe1);
    double Tpipe2 = temperatureNeeded(effect1*length*propFlow*timeStepSecs);
    if (TestNum::ne(Tpipe1, Tpipe2)) {
        QString msg = "Self-test failed, Tpipe1=%1 differs from Tpipe2=%2. Effect=%3.";
        throw Exception(msg.arg(Tpipe1).arg(Tpipe2).arg(effect1), this);
    }
    propFlow = savePropFlow;
}

} //namespace

