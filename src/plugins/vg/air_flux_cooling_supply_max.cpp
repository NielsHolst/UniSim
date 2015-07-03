/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "air_flux_cooling_supply_max.h"
#include "general.h"
#include "publish.h"

using std::min;
using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(AirFluxCoolingSupplyMax)

/*! \class AirFluxCoolingSupplyMax
 * \brief Flux of energy supplied to cool the greenhouse down towards the ventilation setpoint
 * Inputs
 * ------
 * - _ventilationDemand_ is the flux of energy demanded to cool the greenhouse down to the ventilation setpoint [W/m<SUP>2</SUP>]
 * - _byWind_ is the air exchange rate that can be caused by outside wind alone [h<SUP>-1</SUP>]
 * - _byTemp_ is the air exchange rate that can be caused by the outside-inside temperature difference alone [h<SUP>-1</SUP>]
 * - _indoorsTemperature_ is the indoors temperature [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the outdoors temperature [<SUP>o</SUP>C]
 * - _averageHeight_ is the average height of the greenhouse [m]
 *
 * Output
 * ------
 * - _value_ is the flux of energy supplied to cool the greenhouse down towards the ventilation setpoint [W/m<SUP>2</SUP>]
 * - _airFlux_ is the air flux that causes the energy flux [h<SUP>-1</SUP>]
 */

AirFluxCoolingSupplyMax::AirFluxCoolingSupplyMax(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, byWind, "./byWind[value]");
    InputRef(double, byTemp, "./byTemp[value]");
    Output(double, value);
}

void AirFluxCoolingSupplyMax::reset() {
    value = 0.;
}

void AirFluxCoolingSupplyMax::update() {
    value = sqrt(sqr(byWind) + sqr(byTemp));
}

} //namespace

