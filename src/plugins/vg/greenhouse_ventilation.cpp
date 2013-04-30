/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include <usbase/exception.h>
#include <usbase/utilities.h>
#include "greenhouse_ventilation.h"

using namespace UniSim;

namespace vg {
	
GreenhouseVentilation::GreenhouseVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("timeStep", &timeStep, 5., this, "Time step of simulation in @F timeUnit units");
    new Parameter<char>("timeUnit", &timeUnit, 'm', this, "Unit of time step 's', 'm' or 'h'");
    new Parameter<double>("Tindoors", &Tindoors, 27., this, "Indoors temperature (oC)");
    new Parameter<double>("Toutdoors", &Toutdoors, 20., this, "Outdoors temperature (oC)");
    new Parameter<double>("windspeed", &windspeed, 0., this, "Outdoors windspeed (m/s)");
    new Parameter<double>("greenhouseHeight", &greenhouseHeight, 4., this, "Height (m)");
    new Parameter<double>("roofRatio", &roofRatio, 1., this, "Roof/Side wall ratio (-)");
    new Parameter<double>("sideRatio", &sideRatio, 1., this, "Side ratio ? (-)");
    new Parameter<double>("windowLength", &windowLength, 2., this, "Window length (m)");
    new Parameter<double>("alphaVentilationMax", &alphaVentilationMax, 44., this, "Max. ventilation opening (degrees)");
    new Parameter<double>("fractionWindows", &fractionWindows, 0.078, this, "Fraction windows [0;1]");
    new Parameter<double>("leakageVentilation", &leakageVentilation, 0.5, this, "Smaller values for better insulation");
    new Parameter<double>("alphaLeeSide", &alphaLeeSide, 0., this, "Ventilation opening on the lee side [0;100]");
    new Parameter<double>("alphaWindSide", &alphaWindSide, 0., this, "Ventilation opening on the wind side [0;100]");
    new Parameter<double>("screenAirTransmission", &screenAirTransmission, 0., this, "Air transmission of screen coverage [0;1]");
    new Variable<double>("windows", &windows, this, "Greenhouse ventilation rate through windows (m3 air/m2 greenhouse/s");
    new Variable<double>("leakage", &leakage, this, "Greenhouse ventilation rate through leakage (m3 air/m2 greenhouse/s");
    new Variable<double>("total", &total, this, "Greenhouse ventilation rate total (m3 air/m2 greenhouse/s");
}

void GreenhouseVentilation::reset() {
    update();
}

void GreenhouseVentilation::update() {
    const double A_Window = 2.85;

    // Ventilation caused by temperature difference between indoors and outdoors
    double dT = fabs(Tindoors - Toutdoors);
    double &L(windowLength);
    double r1 = 25/180*PI,
           r2 = (25 - alphaVentilationMax)/180*PI;
    double phiTemp = 0.6*L/3*sqrt(9.8/283*dT)*pow(0.8*(sin(r1)-sin(r2)), 1.5)*A_Window;

    // Ventilation caused by convection (De Zwart, p. 89)
    double &b(alphaVentilationMax);
    double gl = 2.29e-2*(1-exp(-b/21.1));             // De Jong
    double gw = 1.2e-3*b*exp(b/211);
    double phiWind = windspeed*A_Window*(gl + gw);    // m3 air/s/window

    // Total ventilation
    double maxVent = 0.5*fractionWindows*sqrt(phiTemp*phiTemp + phiWind*phiWind);   // m3/s/m-2
    double ventilationLee = maxVent*alphaLeeSide/100;
    double ventilationWind = maxVent*alphaWindSide/100;
    windows = (ventilationLee + ventilationWind)/2.;

    // Normalised to average wind speed of 4m/s. GCC, p. 181
    leakage = greenhouseHeight*leakageVentilation/3600*timeStepSecs();
    leakage *= windspeed/4.;

    // Reduce leakage ventilation in the roof part, but not the side part.
    leakage *= (screenAirTransmission*roofRatio+sideRatio)/(roofRatio+sideRatio);

    total = windows + leakage;
}

double GreenhouseVentilation::timeStepSecs() const {
    switch (timeUnit) {
        case 's': return timeStep;
        case 'm': return 60*timeStep;
        case 'h': return 60*60*timeStep;
    }
    QString msg = "Time unit of time step must be 's', 'h' or 'm', not '%1'";
    throw Exception(msg.arg(timeUnit));
}


} //namespace

