/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include <usbase/exception.h>
#include "publish.h"
#include "vents_ventilation.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(VentsVentilation)

/*! \class VentsVentilation
 * \brief Calculates air exchange through vents
 *
 * Inputs
 * ------
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _windspeed_ is the outdoors windspeed [m/s]
 * - _ventLength_ is the total lenth of vents [m]
 * - _ventMaxOpening_ is the maximum possible opening [0;180]
 * - _ventProportion_ is the proportion of the (roof?) area that is vents [0;1]
 * - _ventilationLeeSide_ is the opening in the lee side [0;100]
 * - _ventilationWindSide_ is the opening in the wind side [0;100]
 *
 * Outputs
 * ------
 * - _rate_ is rate of air exchange through vents per greenhouse area [m<SUP>3</SUP>/h/m<SUP>2</SUP>]

 * Default dependencies
 * ------------
 * - an _indoors/temperature_ model with a _value_ port
 * - an _outdoors_ model with two ports:
 *   + _temperature_ [<SUP>o</SUP>C]
 *   + _windspeed_ [m/s] [<SUP>o</SUP>C]
 * - a _construction/ventilation_ model with three ports:
 *   + _ventLength_ [m]
 *   + _ventMaxOpening_ [0;180]
 *   + _ventProportion_ [0;1]
 *   + _groundArea_ [m<SUP>2</SUP>]
 * - an _actuators/vents_ model with two child models:
 *   + _leeside_ with a _state_ port [0;100]
 *   + _windside_ with a _state_ port [0;100]
 */

VentsVentilation::VentsVentilation(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, Tindoors, "indoors/temperature[value]");
    InputRef(double, Toutdoors, "outdoors[temperature]");
    InputRef(double, windspeed, "outdoors[windspeed]");
    InputRef(double, ventLength, "construction/ventilation[ventLength]");
    InputRef(double, ventWidth, "construction/ventilation[ventWidth]");
    InputRef(double, ventDensity, "construction/ventilation[ventDensity]");
    InputRef(double, ventMaxOpening, "construction/ventilation[ventMaxOpening]");
    InputRef(double, roofPitch, "construction/geometry[roofPitch]");
    InputRef(double, ventilationLeeSide, "actuators/vents/leeSide[state]");
    InputRef(double, ventilationWindSide, "actuators/vents/windSide[state]");
    Output(double, rate);
}

void VentsVentilation::reset() {
    rate = 0.;
}

inline double sind(double degrees) {
    return sin(degrees/180*PI);
}

inline double sqr(double x) {
    return x*x;
}

void VentsVentilation::update() {
//    double a = phiTemp(ventilationLeeSide),
//           b = phiTemp(ventilationWindSide),
//           c = phiWindLeeSide(),
//           d = phiWindWindSide();
    double angleLeeSide = ventilationLeeSide/100*ventMaxOpening,
           angleWindSide = ventilationWindSide/100*ventMaxOpening,
           sum = sqr(phiTemp(angleLeeSide) + phiTemp(angleWindSide))
               + sqr(phiWindLeeSide(angleLeeSide) + phiWindWindSide(angleWindSide));
    rate = 0.5*ventDensity*sqrt(sum)*3600;
}

// Ventilation rate per window [m3/window/s]
double VentsVentilation::phiTemp(double angle) {
    double dTemp = fabs(Tindoors - Toutdoors),
           H =	ventWidth*(sind(roofPitch) - sind(roofPitch-angle));
    const double Cf = 0.6,
                 g = 9.82,
                 beta = 1/283.;
    return Cf*ventLength/3*sqr(fabs(g*beta*dTemp))*pow(H,1.5);
}

// Ventilation rate per window [m3/window/s]
double VentsVentilation::phiWindWindSide(double angle) {
    double Gw = 1.2e-3*angle*exp(angle/211),
           A0 = ventLength*ventWidth;
    return Gw*A0*windspeed;
}

// Ventilation rate per window [m3/window/s]
double VentsVentilation::phiWindLeeSide(double angle) {
    double Gl = 2.29e-2*(1-exp(-angle/21.1)),
           A0 = ventLength*ventWidth;
    return Gl*A0*windspeed;
}

} //namespace


