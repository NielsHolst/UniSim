/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <usbase/utilities.h>
#include "general.h"
#include "pipe.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
Pipe::Pipe(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("timeStep", &timeStep, 5., this, "Time step of simulation in @F timeUnit units");
    new Parameter<char>("timeUnit", &timeUnit, 'm', this, "Unit of time step 's', 'm' or 'h'");
    new Parameter<double>("Tindoors", &Tindoors, 27., this, "Indoors temperature (oC)");
    new Parameter<double>("Tunheated", &Tunheated, 20., this, "Greenhouse temperature if not heated (oC)");
    new Parameter<double>("heatingDemand", &heatingDemand, 0., this, "Current energy demand for heating (W/m2");
    new Parameter<double>("spHeating", &spHeating, 0., this, "Setpoint for heating (oC)");
    new Parameter<double>("otherPipesHeatTransfer", &otherPipesHeatTransfer, 0., this, "Heat transfer from other pipes (W/m2)");
    new Parameter<bool>("isEnergyScreenOpening", &isEnergyScreenOpening, false, this, "Is energy screen opening?");
    new Parameter<QString>("pipeType", &pipeType, QString("s51"), this, "Pipe type: " + pipeTypes());
    new Parameter<double>("pipeLength", &pipeLength, 50., this, "Pipe temperature (oC)");
    new Parameter<double>("Tmax", &Tmax, 70., this, "Max pipe temperature (oC)");
    new Parameter<double>("heatParameter", &heatParameter, 1.42, this, "Heat parameter (?)");
    new Parameter<double>("heatTransferParameter", &heatTransferParameter, 0.50, this, "Heat transfer parameter (?)");
    new Parameter<double>("pidK", &pidK, 25., this, "PID control parameter (-)");
    new Parameter<double>("pidRho", &pidRho, 0.003, this, "PID control parameter (s)");

    new Variable<double>("temperature", &temperature, this, "Pipe temperature (oC)");
    new Variable<double>("heatTransfer", &heatTransfer, this, "Heat transfer (W/m2)");
    new Variable<double>("heatEnergy", &heatEnergy, this, "Energy used for heating (J/m2)");

    par["p25"] = Par(0.005, 0.71, -1.8);
    par["s26"] = Par(0.0054, 0.9327, -2.1607);
    par["s33"] = Par(0.006, 1.169, -2.5357);
    par["s51"] = Par(0.009, 1.6232, -2.0893);
}

QString Pipe::pipeTypes() {
    return QStringList(par.keys()).join(", ");
}

void Pipe::reset() {
    update();
}

void Pipe::update() {
    bool isHeating = (Tunheated < spHeating);
    temperature = isHeating ? heatingTemperature() : equilibrate(Tunheated, temperature);
    heatTransfer = pipeLength*parabola(pipeType, temperature - Tindoors);
    heatEnergy = isHeating ? heatTransfer*dtSec() : 0.;
}

double Pipe::heatingTemperature() {
    if (Tunheated > spHeating) {
        temperature = equilibrate(Tunheated, temperature);
        return temperature; // correct?
    }
    double netDemand = heatingDemand - otherPipesHeatTransfer;
    double Tnew = Tindoors + netDemand/(0.006*netDemand + 1.7)/heatParameter;
    double dT = spHeating - Tindoors;

    // PID controller. GCC p. 227ff K_T and rho_T were trained to a greenhouse // OKO 08/2012
    double rho_i = 2*dtSec();
    double rho_d = 0.5*dtSec();
    double Kc = 1.2/pidK * pidRho/dtSec();
    double ut = Kc*(dT + rho_i*dT*dtSec() + rho_d*(temperature - Tnew)/dtSec());
    Tnew += ut;
    Tnew = minMax(spHeating, Tnew, Tmax);

    // Adjustments
    if (Tnew < temperature)
        Tnew = equilibrate(Tnew, temperature);
    if (isEnergyScreenOpening && Tnew < 57.)
        Tnew += 5.;
    return Tnew;
}

double Pipe::equilibrate(double Told, double Tnew) const {
    double c = 60/dtMin();
    return max(Tnew, (Told+T0)*c/(1+c) - T0);
}

double Pipe::dtSec() const {
    return convertTime(timeStep, timeUnit, 's');
}

double Pipe::dtMin() const {
    return convertTime(timeStep, timeUnit, 'm');
}

double Pipe::parabola(QString name, double dT) {
    Par p = par.value(name);
    return p.a*dT*dT + p.b*dT + p.c;
}

} //namespace

