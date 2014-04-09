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

using namespace std;
using namespace UniSim;

namespace vg {
	
HeatPipe::HeatPipe(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameter<double>(Name(length), 200., "Pipe length (m)");
    addParameter<double>(Name(diameter), 51., "Pipe inner diameter (mm)");
    addParameter<double>(Name(flow), 20., "Water flow through pipe (m3/h) when pump is fully on");
    addParameterRef<double>(Name(pumpControl), "controllers/heating/flow[signal]");
    addParameterRef<double>(Name(inflowTemperature), "controllers/heating/temperature[signal]");
    addParameterRef<double>(Name(indoorsTemperature), "indoors/temperature[value]");
    addParameterRef<double>(Name(timeStepSecs), "calendar[timeStepSecs]");
    addVariable<double>(Name(temperature), "Pipe temperature (oC)");
    addVariable<double>(Name(energyEmitted), "Energy emitted (J)");
}

void HeatPipe::initialize() {
    double x = minMax(26., diameter, 51.);
    b = -0.0172*x + 1.567;
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
    temperature = inflowTemperature;
    double r = diameter/2.;
    volume = length*PI*r*r*10./100/100; // liter
    double flowVolume = pumpControl*flow*timeStepSecs*1000./3600;  // liter
    propFlow = min(flowVolume/volume,1.);
    selfTest();
}

void HeatPipe::update() {
    double avgTemp = propFlow*inflowTemperature + (1 - propFlow)*temperature;
    energyEmitted = effect(avgTemp)*length*timeStepSecs;
    temperature = avgTemp - energyEmitted/CpWater/volume;

    //    netDeltaE = DeltaE - loss
//    DeltaTgh = netDeltaE/Cair/Vgh
//    Tgh = Tgh + DeltaTgh
}

//! Compute effect of pipe (W/m), given the pipe temperature \em Tpipe (oC)
double HeatPipe::effect(double Tpipe) {
    double Tdiff = Tpipe - indoorsTemperature;
    return (Tdiff < 0.) ? 0. : (a*Tdiff+b)*Tdiff;
}

//! Compute pipe temperature (oC) needed to emit the given amount of \em energy (J)
double HeatPipe::temperatureNeeded(double energy) {
    double Tdiff = 0;
    if (energy > 0.) {
        double effectNeeded = energy/length/timeStepSecs;
        double D = b*b + 4*a*effectNeeded;
        Tdiff = (-b + sqrt(D))/2./a;
    }
    return indoorsTemperature + Tdiff;
}

void HeatPipe::selfTest() {
    const double Tpipe1 = 20.;
    double effect1 = effect(Tpipe1);
    double Tpipe2 = temperatureNeeded(effect1*length*timeStepSecs);
    if (TestNum::ne(Tpipe1, Tpipe2)) {
        QString msg = "Self-test failed, Tpipe1=%1 differs from Tpipe2=%2. Effect=%3.";
        throw Exception(msg.arg(Tpipe1).arg(Tpipe2).arg(effect1), this);
    }
}

} //namespace

