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

/*! \class FloorTemperature
 * \brief Temperature of the greenhouse floor
 *
 * Inputs
 * ------
 * - _soilTemperature_ is the temperature of outdoors soil (heat sink) [<SUP>o</SUP>C]
 * - _fluxToFloor_ is the energy flux into the floor [W/m<SUP>2</SUP>C]
 * - _timeStep_ is the integration time step [s]
 * - _Usoil_ is the heat transfer coefficient from floor to soil [W/m<SUP>2</SUP>/K]
 * - _Cfloor_ is the heat capacity of the floor [J/m<SUP>2</SUP>/K]
 * - _initValue_ is the initial floor temperature [<SUP>o</SUP>C]
 *
 * Outputs
 * ------
 * - _value_ is the temperature of the greenhouse floor [<SUP>o</SUP>C]

 * Default dependencies
 * ------------
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 * - an _energy/floor_ model with a _flux_ port [W/m<SUP>2</SUP>C]
 * - an _outdoors_ model with a _soilTemperature_ port [<SUP>o</SUP>C]
 */
FloorTemperature::FloorTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, initValue, 21.);
    Input(double, Usoil, 6.);
    Input(double, Cfloor, 42000.);
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, fluxToFloor, "energy/floor[flux]");
    InputRef(double, soilTemperature, "outdoors[soilTemperature]");
    Output(double, value);
}

void FloorTemperature::reset() {
    value = initValue;
}

void FloorTemperature::update() {
    double fluxFromSoil = Usoil*(soilTemperature - value);
    value += (fluxFromSoil - fluxToFloor)*timeStep/Cfloor; // K = W/m2 * s / J/m2/K
}

} //namespace

