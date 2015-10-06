/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "energy_flux_heating_supply.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxHeatingSupply)

/*! \class EnergyFluxHeatingSupply
 * \brief Flux of energy supplied to heat the greenhouse
 * Inputs
 * ------
 * - _maxHeating_ is the maximum heating that can be supplied [W/m<SUP>2</SUP>]
 * - _demand_ is the heating needed [W/m<SUP>2</SUP>]
 *
 * Output
 * ------
 * - _value_ is the heating supplied [W/m<SUP>2</SUP>]
 */

EnergyFluxHeatingSupply::EnergyFluxHeatingSupply(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    Input(double, maxHeating, 10000.);
    InputRef(double, demand, "heating/demand[value]");
}

void EnergyFluxHeatingSupply::update() {
    value = min(demand, maxHeating);
}

} //namespace

