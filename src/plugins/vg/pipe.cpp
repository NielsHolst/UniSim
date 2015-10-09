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
#include "pipe.h"
#include "publish.h"

using std::min;
using std::max;
using namespace UniSim;

namespace vg {
	
PUBLISH(Pipe)

/*! \class Pipe
 * \brief Heat pipe temperature and energy flux
 *
 * The model automatically looks up other Pipe models among its sibling models. This model aims to supply the energy flux,
 * not provided by preceeding Pipe models, to meet the over all goal of _energyFluxTotal_.
 *
 * Inputs
 * ------
 * - _length_ is pipe length per greenhouse area [m/m<SUP>2</SUP>]
 * - _diameter_ is the pipe inner diameter [mm]
 * - _minTemperature_ is the minimum pipe temperature [<SUP>o</SUP>C]
 * - _maxTemperature_ is the maximum pipe temperature [<SUP>o</SUP>C]
 * - _maxTemperatureIncreaseRate_ is the maximum rate at which pipe temperature can increase[<SUP>o</SUP>C/min]
 * - _emissivity_ is the emissivity of the heat pipe [0;1]
 * - _energyFluxTotal_ is the total energy flux to be produced by all pipes [W/m<SUP>2</SUP>]
 * - _indoorsTemperature_ is the indoors temperature [<SUP>o</SUP>C]
 * - _timeStep_ is the integration time step [s]
 *
 * Outputs
 * ------
 * - _temperature_ is the pipe temperature [<SUP>o</SUP>C]
 * - _energyFlux_ is the energy flux coming from this pipe [W/m<SUP>2</SUP>]
 * - _nextTemperatureMin_ is the minimum pipe temperature possible in the next time step [<SUP>o</SUP>C]
 * - _nextTemperatureMax_ is the maximum pipe temperature possible in the next time step [<SUP>o</SUP>C]
 * - _nextEnergyFluxMin_ is the minimum energyFLux possible in the next time step [W/m<SUP>2</SUP>]
 * - _nextEnergyFluxMax_ is the maximum energyFLux possible in the next time step [W/m<SUP>2</SUP>]
 */

Pipe::Pipe(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, length, 1.);
    Input(double, diameter, 51.);
    Input(double, minTemperature, 20.);
    Input(double, maxTemperature, 80.);
    Input(double, maxTemperatureIncreaseRate, 2.);
    Input(double, emissivity, 0.9);
    InputRef(double, energyFluxTotal, "heating/supply[value]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");

    Output(double, temperature);
    Output(double, energyFlux);
    Output(double, nextTemperatureMin);
    Output(double, nextTemperatureMax);
    Output(double, nextEnergyFluxMin);
    Output(double, nextEnergyFluxMax);
}

void Pipe::initialize() {
    auto preceedingPipes = seekPrecedingSiblings();
    for (Model *pipe : preceedingPipes) {
        energyFluxFromPreceedingPipes << pipe->pullValuePtr<double>("energyFlux");
    }
}

QList<Model *> Pipe::seekPrecedingSiblings() {
    Model *parent = seekParent<Model*>("*");
    auto siblings = parent->seekChildren<Model*>("*");
    while (siblings.last() != this)
        siblings.takeLast();
    siblings.takeLast();
    return siblings;
}

void Pipe::reset() {
    double d = minMax(26., diameter, 51.);
    slope = length*(0.0131*d + 0.105);
//    temperature = minTemperature;
    update();
}

void Pipe::update() {
    // Set temperature
    temperature = minMax(minTemperature,
                         indoorsTemperature +
                         calcTemperatureDifference(energyFluxTotal - energyFluxFromPreceedingPipesSum()),
                         maxTemperature);
    nextTemperatureMax = minMax(minTemperature,
                                temperature + maxTemperatureIncreaseRate*timeStep/60.,
                                maxTemperature);
    setNextTemperatureMin();
    // Set energy flux
    energyFlux = calcEnergyFlux(temperature - indoorsTemperature);
    nextEnergyFluxMax = calcEnergyFlux(nextTemperatureMax - indoorsTemperature);
    nextEnergyFluxMin = calcEnergyFlux(nextTemperatureMin - indoorsTemperature);
}

//! Heat flux from heat pipe [W/m<SUP>2</SUP>]
double Pipe::calcEnergyFlux(double temperatureDifference) const {
    // flux = slope*Tdiff^1.25
    return TestNum::leZero(temperatureDifference) ? 0. : slope*pow(temperatureDifference,exponent);
}

//! Temperature difference to obtain a certain heat flux [K]
double Pipe::calcTemperatureDifference(double energyFlux) const {
    // Tdiff = (flux/slope)^(1/1.25)
    return TestNum::leZero(energyFlux) ? 0. :pow(energyFlux/slope, 1./exponent);
}

void Pipe::setNextTemperatureMin() {
    double energyFlux = calcEnergyFlux(temperature - indoorsTemperature),
           waterVolume = length*PI*sqr(diameter/1000./2.);
    nextTemperatureMin = minMax(minTemperature,
                                temperature - energyFlux*timeStep/CpWaterVol/waterVolume,    // K = W/m2 * s * K*m3/J * m2/m3
                                maxTemperature);
}

double Pipe::energyFluxFromPreceedingPipesSum() {
    double sum{0};
    for (const double *energyFlux : energyFluxFromPreceedingPipes) {
        sum += (*energyFlux);
    }
    return sum;
}


} //namespace


