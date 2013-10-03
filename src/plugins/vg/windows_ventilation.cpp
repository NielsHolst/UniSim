/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include "windows_ventilation.h"

using namespace UniSim;

namespace vg {
	
WindowsVentilation::WindowsVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(Tindoors), "climate/temperature[value]");
    addParameterRef<double>(Name(Toutdoors), "environment[temperature]");
    addParameterRef<double>(Name(windspeed), "environment[windspeed]");
    addParameterRef<double>(Name(windowLength), "construction[windowLength]");
    addParameterRef<double>(Name(alphaVentilationMax), "construction[alphaVentilationMax]");
    addParameterRef<double>(Name(fractionWindows), "construction[fractionWindows]");
    addParameterRef<double>(Name(ventilationLeeSide), "regulation/ventilation/leeSideControl[state]");
    addParameterRef<double>(Name(ventilationWindSide), "regulation/ventilation/windSideControl[state]");
    addVariable<double>(Name(value), "Greenhouse ventilation rate through windows (m3 air/m2 greenhouse/s");
}

void WindowsVentilation::reset() {
    value = 0.;
}

void WindowsVentilation::update() {
    const double A_Window = 2.85;

    // Ventilation caused by temperature difference between indoors and outdoors
    double dT = fabs(Tindoors - Toutdoors);
    double &L(windowLength);
    double r1 = 25./180.*PI,
           r2 = (25. - alphaVentilationMax)/180.*PI;
    double phiTemp = 0.6*L/3*sqrt(9.8/283*dT)*pow(0.8*(sin(r1)-sin(r2)), 1.5)*A_Window;

    // Ventilation caused by convection (De Zwart, p. 89)
    double &b(alphaVentilationMax);
    double gl = 2.29e-2*(1. - exp(-b/21.1));             // De Jong
    double gw = 1.2e-3*b*exp(b/211.);
    double phiWind = windspeed*A_Window*(gl + gw);    // m3 air/s/window

    // Total ventilation
    double maxRate = 0.5*fractionWindows*sqrt(phiTemp*phiTemp + phiWind*phiWind);   // m3/m2/s
    value = maxRate*(ventilationLeeSide + ventilationWindSide)/2./100.;
}

} //namespace

