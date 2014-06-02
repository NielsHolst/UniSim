/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "floor_temperature.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(FloorTemperature)

/*! \class CoverTemperature
 * \brief Temperature of greenhouse floor
 *
 * Inputs
 * ------
 * - _soilTemperature_ is the temperature of outdoors soil (heat sink) [<SUP>o</SUP>C]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _Usoil_
 * - _Cfloor_
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
FloorTemperature::FloorTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, initValue, 21.);
    Input(double, Usoil, 6.);
    Input(double, Cfloor, 42000.);
    InputRef(double, fluxToFloor, "energy/floor[flux]");
    InputRef(double, soilTemperature, "outdoors[soilTemperature]");
    Output(double, value);
}

void FloorTemperature::reset() {
    value = initValue;
}

void FloorTemperature::update() {
    double fluxFromSoil = Usoil*(soilTemperature - value);
    value += (fluxFromSoil - fluxToFloor)/Cfloor;
}

} //namespace

