/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_screen_balance.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(EnergyScreenBalance)

/*! \class EnergyScreenBalance
 * \brief Calculates the energy balance for drawing a screen
 *
 * Inputs
 * ------
 * - _netK_ is the combined net K-value of the greenhouse cover  [W/K]
 * - _indoorsRadiation_ is the intensity of indoors radiation [W/m<SUP>2</SUP>]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _greenhouseArea_ is the ground area of the greenhouse [m<SUP>2</SUP>]
 *
 * Output
 * -----
 * - _value_ is the energy balance, negative if energy is lost [W]
 * - _on_ is true if the energy balance is negative [true/false]
 *
 * Default dependencies
 * ------------
 * - an _indoors/cover/transmission model with a _netK_ port [W/K]
 * - an _indoors/radiation_ model with a _total_ port [W/m<SUP>2</SUP>]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>o</SUP>C]
 * - an _outdoors_ model with a _temperature_ port [<SUP>o</SUP>C]
 * - a _construction/geometry_ model with a _groundArea_ port [m<SUP>2</SUP>]
 */

EnergyScreenBalance::EnergyScreenBalance(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, netK, "indoors/cover/transmission[netK]");
    InputRef(double, indoorsRadiation, "indoors/radiation[total]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, greenhouseArea, "construction/geometry[groundArea]");
    Output(double, value);
    Output(bool, on);
}

void EnergyScreenBalance::reset() {
    value = 0;
}

void EnergyScreenBalance::update() {
    value = indoorsRadiation*greenhouseArea - netK*(indoorsTemperature - outdoorsTemperature);
    on = value < 0.;
}

} //namespace
