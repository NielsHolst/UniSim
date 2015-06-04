/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "indoors_temperature.h"
#include "publish.h"

using namespace UniSim;
using std::max;

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
    InputRef(double, heatBalance,"indoors/energy[valueWithoutHeating]");
    InputRef(double, averageHeight,"construction/geometry[averageHeight]");
    InputRef(double, timeStep,"calendar[timeStepSecs]");
    InputRef(double, indoorsAh,"indoors/humidity[ah]");
    InputRef(double, heatingSetpoint, "setpoints/temperature/heating[value]");
    Output(double, value);
    Output(double, virtualTemperature);
}


void IndoorsTemperature::reset() {
    value = virtualTemperature = initValue;
}

void IndoorsTemperature::update() {
    double Cair = averageHeight*RhoAir*CpAir, // J/m2/K = m * kg/m3 * J/kg/K
            tempWithoutHeating = value + heatBalance*timeStep/Cair;
    value = max(tempWithoutHeating, heatingSetpoint);
    virtualTemperature = virtualTemperatureFromAh(value, indoorsAh);
//    if (value < -80 || value > 100) {
//        QString msg{"Temperature is outside reasonable value: %1"};
//        throw Exception(msg.arg(value), this);
//    }
}

//void IndoorsTemperature::update() {
//    double Cair = averageHeight*RhoAir*CpAir; // J/m2/K = m * kg/m3 * J/kg/K
//    value += heatBalance*timeStep/Cair;
//    virtualTemperature = virtualTemperatureFromAh(value, indoorsAh);
//    if (value < -80 || value > 100) {
//        QString msg{"Temperature is outsise reasonable value: %1"};
//        throw Exception(msg.arg(value), this);
//    }
//}

} //namespace
