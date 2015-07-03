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
 * - _indoorsLight_ is the intensity of indoors light [W/m<SUP>2</SUP>]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 */

EnergyScreenBalanceSignal::EnergyScreenBalanceSignal(Identifier name, QObject *parent)
    : BaseSignal(name, parent)
{
    InputRef(double, Ucover, "greenhouseShelter/total[Ucover]");
    InputRef(double, indoorsLight, "indoors/light[total]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
}

double EnergyScreenBalanceSignal::signal()  {
    return (indoorsLight > Ucover*(indoorsTemperature - outdoorsTemperature)) ? 0 : 1;
}

} //namespace
