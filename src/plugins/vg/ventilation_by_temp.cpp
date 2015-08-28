/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "ventilation_by_temp.h"
#include "general.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(VentilationByTemp)

/*! \class VentilationByTemp
 * \brief Flux of energy needed to cool down to ventilation setpoint
 * Inputs
 * ------
 * - _passiveTemperature_ is the indoors tempeature reached, it not actively regulated [<SUP>o</SUP>C]
 * - _heatingSetpoint_ is the heating setpoint [<SUP>oC</SUP>C]
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _timeStep_ is the integration time step [s]
 *
 * Output
 * ------
 * - _value_ is the energy flux needed to heat the greenhouse to the heating setpoint [W/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - an _passive/temperature_ model with a _value_ port [<SUP>oC</SUP>C]
 * - a _setpoints/temperature/heating_ model with a _signal_ port [<SUP>oC</SUP>C]
 * - a _geometry_ model with an _averageHeight_ port [m]
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 */

VentilationByTemp::VentilationByTemp(Identifier name, QObject *parent)
    : Model(name, parent)
{
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, windowLength, "construction/vents[totalLength]");
    InputRef(double, windowHeight, "construction/vents[averageHeight]");
    InputRef(double, greenhouseArea, "geometry[groundArea]");
    Input(double, dischargeCoefficient, 0.7);
    Output(double, value);
}

void VentilationByTemp::reset() {
    value = 0.;
}

// De Jong (1990), eq. 6.10, p.83
void VentilationByTemp::update() {
    double dT = indoorsTemperature - outdoorsTemperature;
    double beta = 1./(indoorsTemperature + T0);
    value = dT<0 ? 0. : dischargeCoefficient*windowLength/3*sqrt(g*beta*dT)*pow(windowHeight, 1.5)*3600./greenhouseArea;
}

} //namespace

