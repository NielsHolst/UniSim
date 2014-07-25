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
 * - an _effectuators/vents_ model with two child models:
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
    InputRef(double, ventilationLeeSide, "effectuators/vents/leeSide[state]");
    InputRef(double, ventilationWindSide, "effectuators/vents/windSide[state]");
    Output(double, rate);
}

void VentsVentilation::reset() {
    rate = 0.;
}

void VentsVentilation::update() {
    const double standardVentilation = 0.078*2.85;
    double ventArea = ventLength*ventWidth,
           thisVentilation = ventDensity*ventArea,
           calibration = pow(standardVentilation/thisVentilation, 1.5);
    // NumVentsRoof?

    // Ventilation caused by temperature difference between indoors and outdoors
    double dTemp = fabs(Tindoors - Toutdoors);
    double &L(ventLength);
    double &b(ventMaxOpening);
    double r1 = 25./180.*PI,
           r2 = (25. - b)/180.*PI;
    double phiTemp = 0.6*L/3*sqrt(9.8/283*dTemp)*pow(0.8*(sin(r1)-sin(r2)), 1.5)*ventArea;

    // Ventilation caused by convection (De Zwart, p. 89)
    double gl = 2.29e-2*(1. - exp(-b/21.1));             // De Jong
    double gw = 1.2e-3*b*exp(b/211.);
    double phiWind = windspeed*ventArea*(gl + gw);    // m3 air/s/window

    // Total ventilation
    double maxRate = 0.5*ventDensity*sqrt(phiTemp*phiTemp + phiWind*phiWind);   // m3/m2/s
    rate = maxRate*(ventilationLeeSide + ventilationWindSide)/2./100.;
    rate *= 3600;
}

} //namespace


