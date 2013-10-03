/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "control_element.h"
#include "general.h"
#include "pipe.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
Pipe::Pipe(Identifier name, QObject *parent)
	: Model(name, parent)
{
    StringMapEntry(pipeTypes, p25);
    StringMapEntry(pipeTypes, s26);
    StringMapEntry(pipeTypes, s33);
    StringMapEntry(pipeTypes, s51);

    addParameter<QString>(Name(pipeType), QString("s51"),
                          "Possible values: " + pipeTypes.validKeys().join(", "));
    addParameterRef<double>(Name(heatingDemand), "heating/demand/pipe[signal]");
    addParameterRef<double>(Name(Tunheated), "greenhouse/energetics[Tunheated]");
    addParameterRef<int>(Name(energyScreenCourse), "screens/energy/control[course]");
    addParameterRef<double>(Name(Tminimum), "setpoints/temperature[minimum]");
    addParameterRef<double>(Name(Tindoors), "climate/temperature[value]");
    addParameterRef<double>(Name(actualTpipe), "./control[state]");

    addParameter<double>(Name(pipeLength), 1.822, "Pipe length (m/m2)");
    addParameter<double>(Name(TmaxPipe), 70., "Max pipe temperature (oC)");

    addVariable<double>(Name(signal), "Desired pipe temperature (oC)");
    addVariable<double>("heatFlux", &_heatFlux, "Heat flux from pipe (W/m2)");
}

void Pipe::initialize() {
    switch (pipeTypes.seek(pipeType, this)) {
    case p25:
        a = 0.005; b = 0.71; c = -1.8;
        break;
    case s26:
        a = 0.0054; b = 0.9327; c = -2.1607;
        break;
    case s33:
        a = 0.006; b = 1.169; c = -2.5357;
        break;
    case s51:
        a = 0.009; b = 1.6232; c = -2.0893;
        break;
    default:
        throw Exception("Unknown pipe type", this);
    }
    selfTest();
}

void Pipe::reset() {
    signal = 20.;
    _heatFlux = heatFlux(signal);
}

void Pipe::update() {
    signal = (heatingDemand <= 0.) ?
                Tunheated :
                Tpipe(heatingDemand) /*+ energyScreenCorrection()*/;
    signal =  minMax(Tminimum, signal, TmaxPipe);
    _heatFlux = heatFlux(actualTpipe);
}

double Pipe::energyScreenCorrection() {
    return (energyScreenCourse == int(ControlElement::Increasing)) ? 5. : 0.;
}

double Pipe::heatFlux(double Tpipe) {
    double dT = Tpipe - Tindoors;
    return (dT < 0.) ? 0. : max(pipeLength*(a*dT*dT + b*dT + c), 0.);
}

double Pipe::Tpipe(double heatFlux) {
    Q_ASSERT(heatFlux > 0.);
    double A = pipeLength*a,
            B = pipeLength*b,
            C = pipeLength*c - heatFlux;
    double D = B*B - 4*A*C;
    Q_ASSERT(D>0.);
    double dT = (-B + sqrt(D))/2./A;
    return Tindoors + dT;
}

void Pipe::selfTest() {
    const double Tpipe_ = Tindoors + 5.;
    double flux = heatFlux(Tpipe_);
    double Tpipe2 = Tpipe(flux);
    if (TestNum::ne(Tpipe_, Tpipe2)) {
        QString msg = "Self-test failed, Tpipe=%1 differs from Tpipe2=%2. Heat flux=%3.";
        throw Exception(msg.arg(Tpipe_).arg(Tpipe2).arg(flux));
    }
}

} //namespace

