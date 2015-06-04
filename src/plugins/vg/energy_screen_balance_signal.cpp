/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_screen_balance_signal.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(EnergyScreenBalanceSignal)

/*! \class EnergyScreenBalanceSignal
 * \brief Calculates the energy balance for drawing a screen
 *
 * Inputs
 * ------
 * - _Ucover_ is the average heat transfer coefficient of the greenhouse cover per ground area [W/K/m<SUP>2</SUP>]
 * - _indoorsRadiation_ is the intensity of indoors radiation [W/m<SUP>2</SUP>]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 *
 *
 * Default dependencies
 * ------------
 * - an _indoors/cover/transmission model with a _netK_ port [W/K]
 * - an _indoors/radiation_ model with a _total_ port [W/m<SUP>2</SUP>]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>o</SUP>C]
 * - an _outdoors_ model with a _temperature_ port [<SUP>o</SUP>C]
 */

EnergyScreenBalanceSignal::EnergyScreenBalanceSignal(Identifier name, QObject *parent)
    : BaseSignal(name, parent)
{
    InputRef(double, Ucover, "greenhouseShelter[Ucover]");
    InputRef(double, indoorsRadiation, "indoors/radiation[total]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
}

double EnergyScreenBalanceSignal::signal()  {
    return (indoorsRadiation > Ucover*(indoorsTemperature - outdoorsTemperature)) ? 0 : 1;
}

} //namespace
