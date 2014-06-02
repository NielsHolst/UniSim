/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_floor.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxFloor)

/*! \class EnergyFluxSurface
 * \brief Flux of energy through greenhouse floor
 * Inputs
 * ------
 * - _U_ is the U-value from greenhouse air to floor [W/m<SUP>2</SUP>]/K]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>oC</SUP>C]
 * - _floorTemperature_ is the temperature of the floor [<SUP>oC</SUP>C]
 *
 * Output
 * ------
 * - _flux_ is the flux of energy through the greenhouse surface [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _construction/floor_ model with a _U_ port [W/m<SUP>2</SUP>]/K]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>oC</SUP>C]
 * - an _indoors/floor/temperature_ model with a _aue_ port [<SUP>oC</SUP>C]
 */

EnergyFluxFloor::EnergyFluxFloor(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, U, "construction/floor[U]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, floorTemperature, "indoors/floor/temperature[value]");
    Output(double, flux);
}

void EnergyFluxFloor::reset() {
    flux = 0.;
}

void EnergyFluxFloor::update() {
    flux = U*(floorTemperature-indoorsTemperature);
}

} //namespace

