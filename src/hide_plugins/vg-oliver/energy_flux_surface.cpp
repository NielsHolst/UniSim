/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_surface.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxSurface)

/*! \class EnergyFluxSurface
 * \brief Flux of energy through greenhouse surface (cover+screens)
 * Inputs
 * ------
 * - _netU_ is the combined U-value of cover and screens per greenhouse area [W/m<SUP>2</SUP>]/K]
 * - _indoorsTemperature_ the ambient temperature indoors [<SUP>oC</SUP>C]
 * - _outdoorsTemperature_ the ambient temperature outdoors [<SUP>oC</SUP>C]
 *
 * Output
 * ------
 * - _flux_ is the flux of energy through the greenhouse surface [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _screens/transmission_ model with a _U_ port [W/m<SUP>2</SUP>]/K]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>oC</SUP>C]
 * - an _outdoors_ model with a _temperature_ port [<SUP>oC</SUP>C]
 */

EnergyFluxSurface::EnergyFluxSurface(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, netU, "screens/transmission[netU]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    Output(double, flux);
}

void EnergyFluxSurface::reset() {
    flux = 0.;
}

void EnergyFluxSurface::update() {
    flux = netU*(outdoorsTemperature-indoorsTemperature);
}


} //namespace

