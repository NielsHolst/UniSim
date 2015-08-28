/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "publish.h"
#include "co2_controller.h"

using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(Co2Controller)

/*! \class Co2Controller
 * \brief Sets the rate of CO2 enrichment
 *
 * Inputs
 * ------
 * - _assimilation_ tells plant CO2 assimilation rate [g/m<SUP>2</SUP>/h]
 * - _ventilation_ tells the relative ventilation rate within a time step [0;1]
 * - _minCo2_ is the minimum set point for CO2 in the greenhouse [ppm]
 * - _indoorsCo2_ is the indoors CO2 level [ppm]
 * - _outdoorsCo2_ is the outdoors CO2 level [ppm]
 * - _indoorsTemperature_ is the indoors temperature [<SUP>o</SUP>C]
 * - _timeStep_ is the duration of one integration time step [s]
 * - _height_ is the average greenhouse height [m]
 * - _maxSignal_ is the maximum CO2 enrichment rate [g/m<SUP>2</SUP>/h]
 *
 * Output
 * -----
 * - _signal_ is the desired CO2 enrichment rate [g/m<SUP>2</SUP>/h]
 *
 */

Co2Controller::Co2Controller(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, assimilation, "co2Uptake[signal]");       //Dummy model
    InputRef(double, minCo2, "setpoints/minCo2[setpoint]");
    InputRef(double, indoorsCo2, "indoors/co2[ppm]");
    InputRef(double, outdoorsCo2, "outdoors[co2]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, height, "geometry[indoorsAverageHeight]");
    Input(double, maxSignal, 20);
    Output(double, signal);
}

void Co2Controller::reset() {
    signal = 0.;
}

void Co2Controller::update() {
    double ventilationBalance = (indoorsCo2*(1-ventilation) + outdoorsCo2*ventilation)*timeStep/3600 - minCo2;  // ppm
    double demand = assimilation - absFromPpmCo2(indoorsTemperature,ventilationBalance)/timeStep*3600*height; // g/m2/h
    signal = min(demand, maxSignal);
}

} //namespace
