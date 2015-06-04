/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <math.h>
#include <usbase/exception.h>
#include "publish.h"
#include "vent.h"
#include "vents.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(Vents)

/*! \class Vents
 * \brief Calculates air exchange through vents
 *
 * Inputs
 * ------
 * - _length_ is the length of a vents window [m]
 * - _width_ is the width of a vents window [m]
 * - _maxOpening_ is the maximum possible opening [0;180]
 * - _density_ is the number of vent windows per greenhouse area [m<SUP>-2</SUP>]
 * - _porosity_ is the efficacy of ventilation (can be reduced, for example, by insect net) [0;1]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _windspeed_ is the outdoors windspeed [m/s]
 * - _roofPitch_ is the degrees slope of the roof [0;180]
 * - _ventilationLeeSide_ is the opening in the lee side [0;100]
 * - _ventilationWindSide_ is the opening in the wind side [0;100]
 *
 * Outputs
 * ------
 * - _rate_ is rate of air exchange through vents per greenhouse area [m<SUP>3</SUP>/s/m<SUP>2</SUP>]

 * Default dependencies
 * ------------
 * - an _indoors/temperature_ model with a _value_ port
 * - an _outdoors_ model with two ports:
 *   + _temperature_ [<SUP>o</SUP>C]
 *   + _windspeed_ [m/s] [<SUP>o</SUP>C]
 * - a _construction/ventilation_ model with three ports:
 *   + _ventLength_ [m]
 *   + _ventWidth_ [m]
 *   + _ventDensity_ [m<SUP>-2</SUP>]
 *   + _ventMaxOpening_ [0;180]
 *   + _efficacy_ [0;1]
 * - a _construction/geometry_ model with a _roofPitch_ port [0;180]
 * - an _actuators/vents_ model with two child models:
 *   + _leeside_ with a _state_ port [0;100]
 *   + _windside_ with a _state_ port [0;100]
 */

Vents::Vents(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, greenhouseArea, "construction/geometry[groundArea]");
    Output(double, totalLength);
    Output(double, averageHeight);
    Output(double, proportionalEffectiveArea);
}

void Vents::reset() {
    totalLength =
    averageHeight =
    proportionalEffectiveArea = 0;
    auto vents = seekChildren<Vent*>("*");
    for (auto vent : vents) {
        double length = vent->pullValue<double>("length"),
               height = vent->pullValue<double>("height") ;
        totalLength += length;
        averageHeight += height*length;
        proportionalEffectiveArea += vent->pullValue<double>("effectiveArea");
    }
    if (totalLength>0.) averageHeight /= totalLength;
    proportionalEffectiveArea /= greenhouseArea;
}


} //namespace


