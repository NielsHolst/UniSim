/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <usbase/exception.h>
#include <usbase/utilities.h>
#include <usbase/test_num.h>
#include "general.h"
#include "heat_pipe.h"
#include "publish.h"

using std::min;
using std::max;
using namespace UniSim;

namespace vg {
	
PUBLISH(HeatPipe)

/*! \class HeatPipe
 * \brief Computes heat pipe temperature and emittance
 *
 * Inputs
 * ------
 * - _length_ is pipe length per greenhouse area [m/m<SUP>2</SUP>]
 * - _diameter_ is the pipe inner diameter [mm]
 * - _flowRate_ is the flow rate [m<SUP>3</SUP>/h]
 * - _inflowTemperature_ is the temperature of the water flowing into the pipe [<SUP>o</SUP>C]
 * - _indoorsTemperature_ is the ambient temperature of the greenhouse [<SUP>o</SUP>C]
 * - _timeStep_ is the integration time step [s]
 * - _greenhouseArea_ is the ground area covered by the greenhouse [m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _temperature_ is the pipe temperature [<SUP>o</SUP>C]
 * - _effect_ is the energy emission per greenhouse area [W/m<SUP>2</SUP>]

 * Default dependencies
 * ------------
 * - a _controllers/heating/temperature_ model wit a _signal_ port [<SUP>o</SUP>C]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>o</SUP>C]
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 * - a _construction/geometry_ model with a _groundArea_ port [m<SUP>2</SUP>]
 */

HeatPipe::HeatPipe(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, length, 1.);
    Input(double, diameter, 51.);
    Input(double, minTemperature, 20.);
    Input(double, maxTemperature, 80.);
    Input(double, energyFlux, 0.);
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    Output(double, temperature);
    Output(double, minEnergyFlux);
    Output(double, maxEnergyFlux);
}

void HeatPipe::reset() {
    double x = minMax(26., diameter, 51.);
    slope = length*(0.0131*x + 0.105);

    temperature = minTemperature;
    minEnergyFlux = calcEnergyFluxMin();
    maxEnergyFlux = calcEnergyFluxMax();
}

void HeatPipe::update() {
    temperature = calcPipeTemperature(energyFlux);
    minEnergyFlux = calcEnergyFluxMin();
    maxEnergyFlux = calcEnergyFluxMax();
}

double HeatPipe::calcEnergyFluxMin() const {
    return calcEnergyFlux(minTemperature);
}

double HeatPipe::calcEnergyFluxMax() const {
    return calcEnergyFlux(maxTemperature);
}

double HeatPipe::calcEnergyFlux(double pipeTemperature) const {
    // flux = slope*dT^1.25
    double dT = pipeTemperature - indoorsTemperature;
    return (dT <= 0) ? 0. : slope*pow(dT,1.25);
}

double HeatPipe::calcPipeTemperature(double energyFlux) const {
    // dT = (flux/slope)^(1/1.25)
    return indoorsTemperature + pow(energyFlux/slope, 1./exponent);
}




} //namespace


