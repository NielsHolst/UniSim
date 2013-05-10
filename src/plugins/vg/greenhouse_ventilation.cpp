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
    addParameterRef<int>(Name(timeStep), "calendar[timeStep]");
    addParameterRef<char>(Name(timeUnit), "calendar[timeUnit]");
    addParameterRef<double>(Name(Tindoors), "indoors/temperature[air]");
    addParameterRef<double>(Name(Toutdoors), "outdoors/records[Tair]");
    addParameterRef<double>(Name(windspeed), "outdoors/records[windspeed]");
    addParameterRef<double>(Name(greenhouseHeight), "greenhouse[height]");
    addParameterRef<double>(Name(roofRatio), "greenhouse[roofRatio]");
    addParameterRef<double>(Name(sideRatio), "greenhouse[sideRatio]");
    addParameterRef<double>(Name(windowLength), "greenhouse[windowLength]");
    addParameterRef<double>(Name(alphaVentilationMax), "greenhouse[alphaVentilationMax]");
    addParameterRef<double>(Name(fractionWindows), "greenhouse[fractionWindows]");
    addParameterRef<double>(Name(leakageVentilation), "greenhouse[leakageVentilation]");
    addParameterRef<double>(Name(alphaLeeSide), "ventilation/spOpening[alphaLeeSide]");
    addParameterRef<double>(Name(alphaWindSide), "ventilation/spOpening[alphaWindSide]");
    addParameterRef<double>(Name(screenAirTransmission), "greenhouse/screen/airTransmission[transmission]");
    addVariable<double>(Name(windows), "Greenhouse ventilation rate through windows (m3 air/m2 greenhouse/s");
    addVariable<double>(Name(leakage), "Greenhouse ventilation rate through leakage (m3 air/m2 greenhouse/s");
    addVariable<double>(Name(total), "Greenhouse ventilation rate total (m3 air/m2 greenhouse/s");
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
    return convertTime(timeStep, timeUnit, 's');
}


} //namespace

