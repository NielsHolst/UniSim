/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_temperature.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(IndoorsTemperature)

/*! \class IndoorsTemperature
 * \brief Updates indoors temperature based on heat balance
 *
 * Inputs
 * ------
 * - _initValue_ is the initial indoors temperature [<SUP>o</SUP>C]
 * - _Cgreenhouse_ is the heat capacity per greenhouse area of the greenhouse floor and other solid greenhouse components [J/m<SUP>2</SUP>/k]
 * - _heatBalance_ is the heat balance per greenhouse area [W/m<SUP>2</SUP>]
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _value_ is the indoors tempeature [<SUP>o</SUP>C]
 *
 * Default dependencies
 * ------------
 * - an _indoors/ventilation_ model with a _relative_ port [0;1]
 * - an _indoors/heatInfiltration_ model with a _balance_ port [W/m<SUP>2</SUP>]
 * - an _indoors/radiation model with a total port [W/m<SUP>2</SUP>]
 * - an _indoors/screensHeatTransmission model with a _balance_ port [W/m<SUP>2</SUP>]
 */

IndoorsTemperature::IndoorsTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
//    Input(double, Cgreenhouse, 42000);
    Input(double, initValue,21);
    InputRef(double, relativeVentilationRate, "indoors/ventilation[relative]");
    InputRef(double, heatBalance,"indoors/energy[value]");
    InputRef(double, averageHeight,"construction/geometry[averageHeight]");
    InputRef(double, timeStep,"calendar[timeStepSecs]");
    InputRef(double, indoorsAh,"indoors/humidity[ah]");
    Output(double, value);
    Output(double, virtualTemperature);
}


void IndoorsTemperature::reset() {
    value = virtualTemperature = initValue;
}

void IndoorsTemperature::update() {
    double &Vgh(averageHeight),  // m3
           Cair = Vgh*RhoAir*CpAir; // J/m2/K = m3 * kg/m3 * J/kg/K
//    value += heatBalance*timeStep/(Cgreenhouse + Cair);
    value += heatBalance*timeStep/Cair;
    virtualTemperature = virtualTemperatureFromAh(value, indoorsAh);
}

} //namespace
