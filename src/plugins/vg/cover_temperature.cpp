/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "cover_temperature.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(CoverTemperature)

/*! \class CoverTemperature
 * \brief Temperature of greenhouse cove
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
CoverTemperature::CoverTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, latcov, 0.);
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, indoorsAh, "indoors/humidity[ah]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, skyTemperature, "outdoors[skyTemperature]");
    InputRef(double, windspeed, "outdoors[windspeed]");
    Output(double, value);
    Output(double, virtualTemperature);
    Output(bool, hasCondensation);
}

void CoverTemperature::reset() {
    value = virtualTemperature = outdoorsTemperature;
}

void CoverTemperature::update() {
    double alfa_o = (windspeed < 4) ?
                1.2*windspeed + 2.8 //%Bot, 1993
                : pow(2.5*windspeed, 0.8);
    const double alfa_i = 3;

    value = ((6+alfa_i)*(indoorsTemperature+T0) +
                   6*(skyTemperature+T0) +
                   alfa_o*(outdoorsTemperature+T0) + latcov)
            / (12 + alfa_o + alfa_i) - T0;

    value = (indoorsTemperature + outdoorsTemperature + skyTemperature)/3.;
    hasCondensation = sah(value) < indoorsAh;
    virtualTemperature = hasCondensation ?
                         virtualTemperatureFromAh(value, sah(value)) :
                         virtualTemperatureFromAh(value, indoorsAh);
}

} //namespace

