/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_shelter.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxShelter)

/*! \class EnergyFluxShelter
 * \brief Flux of energy through greenhouse shelter (cover+screens)
 * Inputs
 * ------
 * - _netU_ is the combined U-value of cover and screens per greenhouse area [W/m<SUP>2</SUP>]/K]
 * - _indoorsTemperature_ the ambient temperature indoors [<SUP>oC</SUP>C]
 * - _outdoorsTemperature_ the ambient temperature outdoors [<SUP>oC</SUP>C]
 *
 * Default dependencies
 * ------------
 * - a _screens/transmission_ model with a _U_ port [W/m<SUP>2</SUP>]/K]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>oC</SUP>C]
 * - an _outdoors_ model with a _temperature_ port [<SUP>oC</SUP>C]
 */

EnergyFluxShelter::EnergyFluxShelter(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    InputRef(double, netU, "greenhouseShelter[U]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
}

void EnergyFluxShelter::update() {
    value = netU*(outdoorsTemperature-indoorsTemperature);
}


} //namespace

