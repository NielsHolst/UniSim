/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_humidity.h"
#include "publish.h"
#include "vapour_flux_base.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
PUBLISH(IndoorsHumidity)

/*! \class IndoorsHumidity
 * \brief Maintains indoors humidity
 *
 * Inputs
 * ------
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsRh_ is the outdoors absolute humidity [kg/m<SUP>3</SUP>]
 * - _timeStep_ is the integration time step[s]
 * - _averageHeight_ is the average height of the greenhouse (volume divided by ground area) [m]
 *
 * Outputs
 * ------
 * - _rh_ is the indoors relative humidity [0;100]
 * - _ah_ is the indoors absolute humidity [kg/m<SUP>3</SUP>]
 * - _ahEq_ is the asymptotic (with time) equilibrium absolute humidity [kg/m<SUP>3</SUP>]
 * - _timeConstant_ is the time constant used to integrate _netVapourFlux_ [s<SUP>-1</SUP>]
 * - _surplusAh_ is an integration inaccuracy that was rounded off [kg/m<SUP>3</SUP>]
 * - _netVapourFlux_ is the total flux of water vapour in/out (+/-) of the greenhouse [kg/m<SUP>2</SUP>/s]
 * - _simpleSumVapourFlux_ is the sum of all water vapour fluxes; an imprecise calculation of _netVapourFlux_ [kg/m<SUP>2</SUP>/s]
 *
 * Default dependencies
 * ------------
 * - an _indoors/temperature_ model with a _value_ port [<SUP>o</SUP>C]
 * - a _setpoints/humidity_ model with two ports:
 *   + maxRh [0;100]
 *   + minDeltaX [Pa]
 */

IndoorsHumidity::IndoorsHumidity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, conductance, 0.);
    Input(double, vapourFlux, 0.);
    Input(double, gain, 0.);
    Input(double, temperature, 0.);
    Input(double, averageHeight, 0.);
    InputRef(double, timeStep, "calendar[timeStepSecs]");

    Output(double, rh);
    Output(double, ah);
    Output(double, ahEq);
    Output(double, timeConstant);
    Output(double, surplusAh);
    Output(double, netVapourFlux);
}

void IndoorsHumidity::reset() {
    tick = 0;
    rh = 70.;
    ah = ahEq = ahFromRh(temperature, rh);
    netVapourFlux = timeConstant = surplusAh = 0.;
}

void IndoorsHumidity::update() {
    // Keep humidity constant for the first few time steps to stabilise overall model state
    if (tick++ < 10) return;
    double prevAh = ah;
    if (conductance > 0. && gain > 0.) {
        ahEq = gain/conductance;
        timeConstant = averageHeight/gain;
        ah = ahEq - (ahEq-ah)*exp(-timeStep/timeConstant);
    }
    else {
        ahEq = ah;
        timeConstant = 0.;
    }

    double indoorsSah = sah(temperature);
    if (ah > indoorsSah) {
        ah = indoorsSah;
        surplusAh = ah - indoorsSah;
    }
    else {
        surplusAh = 0.;
    }

    rh = rhFromAh(temperature, ah);
    netVapourFlux = (ah - prevAh)*averageHeight/timeStep; // kg/m2/s = kg/m3 * m3/m2 / s
}

} //namespace
