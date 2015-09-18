/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "heat_buffer.h"
#include "publish.h"

using std::min;
using namespace UniSim;

namespace vg {

PUBLISH(HeatBuffer)

/*! \class HeatBuffer
 * \brief Heat buffer for heating system
 *
 * Inputs
 * ------
 * - _volume_ is the buffer volume of water per greenhouse area [m<SUP>3</SUP>/m<SUP>2</SUP>]
 * - _minTemperature_ is the minimum temperature
 * - _U_ is the U-value for heat transmission from the buffer to the greenhouse air [W/m<SUP>2</SUP>/K]
 * - _heatingEnergyFLux_ is the current energy flux supplied by the heating system
 * - _timeStep_ is the integration time step
 *
 * Output
 * ------
 * - _temperature_ is the water temperature in the buffer [<SUP>o</SUP>C]
 * - _energyFlux_ is the heat transmission from the buffer to the greenhouse air [W/m<SUP>2</SUP>]
 * - _energyFluxLag_ is the difference between received and emitted energy flux [W/m<SUP>2</SUP>]
 */

HeatBuffer::HeatBuffer(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, volume, 0.01);
    Input(double, U, 8.5);
    Input(double, minTemperature, 20.);
    Input(double, maxTemperature, 20.);
    InputRef(double, heatingEnergyFlux, "controlled/energyflux/supply[value]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    Output(double, temperature);
    Output(double, energyFlux);
}

void HeatBuffer::reset() {
    temperature = indoorsTemperature;
    energyFlux = 0.;
}

void HeatBuffer::update() {
    double
        diffTemperature = temperature - indoorsTemperature,
        newTemperature =
            (diffTemperature < 0.) ?
                minTemperature :
                temperature - diffTemperature*(1. - exp(-U/volume/CpWaterVol*timeStep)),
                // temperature -> indoorsTemperature for timeStep -> inf
                // temperature -> temperature - diffTemperature*U/volume/CpWaterVol*timeStep for timeStep -> 0
                // K = K * W/m2/K / (m3/m2) / (J/m3/K) * s = K * W/m2/K  * m2/m3 * m3*K/J * s =
                //     J/m3 * m3*K/J = K
        temperatureLoss = temperature - newTemperature;

    energyFlux = temperatureLoss*volume*CpWaterVol/timeStep;
    // W/m2 = K * m3/m2 * (J/m3/K) / s
    energyFluxLag = energyFlux - heatingEnergyFlux;
    temperature = min(newTemperature + heatingEnergyFlux/volume/CpWaterVol*timeStep, maxTemperature);
}


} //namespace

