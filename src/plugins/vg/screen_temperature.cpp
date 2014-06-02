/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_temperature.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(ScreenTemperature)

/*! \class ScreenTemperature
 * \brief Temperature of greenhouse screens
 *
 * Inputs
 * ------
 * - _latcov is what? [0-180]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _skyTemperature_ is the temperature of the sky [<SUP>o</SUP>C]
 * - _windspeed_ is the outdoors windspeed [m/s]
 *
 * Outputs
 * ------
 * - _value_ is the temperature of the greenhouse cover [<SUP>o</SUP>C]

 * Default dependencies
 * ------------
 * - an _indoors/temperature_ model with a _value_ port
 * - an _outdoors_ model with three ports:
 *   + _temperature_ [<SUP>o</SUP>C]
 *   + _skyTemperature_ [<SUP>o</SUP>C]
 *   + _windspeed_ [m/s] [<SUP>o</SUP>C]
 */
ScreenTemperature::ScreenTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, latcov, 0.);
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, coverTemperature, "indoors/cover/temperature[value]");
    Output(double, value);
}

void ScreenTemperature::reset() {
    value = coverTemperature;
}

void ScreenTemperature::update() {
    const double alfa_i = 3,
                 windspeed =0.5,
                 alfa_o = 1.2*windspeed + 2.8; //%Bot, 1993

    value = ((6+alfa_i)*(indoorsTemperature+T0) +
                   6*(coverTemperature+T0) +
                   alfa_o*(indoorsTemperature+T0) + latcov)
            / (12 + alfa_o + alfa_i) - T0;
}

} //namespace

