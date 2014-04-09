/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include "vents_ventilation.h"

using namespace UniSim;

namespace vg {
	
VentsVentilation::VentsVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(Tindoors), "indoors/temperature[value]");
    addParameterRef<double>(Name(Toutdoors), "outdoors[temperature]");
    addParameterRef<double>(Name(windspeed), "outdoors[windspeed]");
    addParameterRef<double>(Name(ventLength), "construction/ventilation[ventLength]");
    addParameterRef<double>(Name(ventMaxOpening), "construction/ventilation[ventMaxOpening]");
    addParameterRef<double>(Name(ventProportion), "construction/ventilation[ventProportion]");
    addParameterRef<double>(Name(ventilationLeeSide), "effectuators/vents/leeSide[state]");
    addParameterRef<double>(Name(ventilationWindSide), "effectuators/vents/windSide[state]");
    addVariable<double>(Name(value), "Greenhouse ventilation rate through windows (m3 air/h");
}

void VentsVentilation::reset() {
    value = 0.;
}

void VentsVentilation::update() {
    const double A_Window = 2.85;

    // Ventilation caused by temperature difference between indoors and outdoors
    double dTemp = fabs(Tindoors - Toutdoors);
    double &L(ventLength);
    double r1 = 25./180.*PI,
           r2 = (25. - ventMaxOpening)/180.*PI;
    double phiTemp = 0.6*L/3*sqrt(9.8/283*dTemp)*pow(0.8*(sin(r1)-sin(r2)), 1.5)*A_Window;

    // Ventilation caused by convection (De Zwart, p. 89)
    double &b(ventMaxOpening);
    double gl = 2.29e-2*(1. - exp(-b/21.1));             // De Jong
    double gw = 1.2e-3*b*exp(b/211.);
    double phiWind = windspeed*A_Window*(gl + gw);    // m3 air/s/window

    // Total ventilation
    double maxRate = 0.5*ventProportion*sqrt(phiTemp*phiTemp + phiWind*phiWind);   // m3/m2/s  ??
    value = maxRate*(ventilationLeeSide + ventilationWindSide)/2./100.;
    value *= 3600;
}

} //namespace

