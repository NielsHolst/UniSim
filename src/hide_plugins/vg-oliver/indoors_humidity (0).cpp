/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_humidity.h"
#include "publish.h"

using namespace std;
using namespace UniSim;

namespace vg {
	
PUBLISH(IndoorsHumidity)

/*! \class IndoorsHumidity
 * \brief Maintains indoors humidity
 *
 * Inputs
 * ------
 * - _propVent_ is the propotion of air being ventilated during one time step [0;1]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _outdoorsRh_ is the relative humidity outdoors [<SUP>o</SUP>C]
 * - _maxRh_ is the r.h. maximum setpoint [0-100]
 * - _minDeltaX_ is the delta-x minimum setpoint [g/m<SUP>3</SUP>]
 *
 * Outputs
 * ------
 * - _rhExcess_ is the excess r.h. relative to the setpoint _maxRh_ [% points]");
 * - _wvpdInsufficiency_ is the insufficiency of vapour pressure deficit relative to the setpoint _minDeltaX_ [Pa]");
 * - _highRh_ tells if r.h. is too high for the setpoint _maxRh_ [true,false]
 * - _lowDeltaX_ tells if the vapour pressure deficit is too small for the setpoint _minDeltaX_ [true,false]
 * - _highHumity_ tells if the humidity is too high according to either setpoint [true,false]
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
    InputRef2(double, conductance.transpiration, "condTrans", "plant/transpiration[conductance]");
    InputRef2(double, conductance.condensation, "condCond", "cover/condensation[conductance]");
    InputRef2(double, conductance.ventilation, "condVent", "indoors/ventilation[conductance]");
    InputRef2(double, gain.transpiration, "gainTrans", "plant/transpiration[gain]");
    InputRef2(double, gain.condensation, "gainCond", "cover/condensation[gain]");
    InputRef2(double, gain.ventilation, "gainVent", "indoors/ventilation[gain]");
    InputRef2(double, grossFlux.transpiration, "gFluxTrans", "plant/transpiration[vapourFlux]");
    InputRef2(double, grossFlux.condensation, "gFluxCond", "cover/condensation[vapourFlux]");
    InputRef2(double, grossFlux.ventilation, "gFluxVent", "indoors/ventilation[vapourFlux]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsAh, "outdoors[ah]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, averageHeight, "construction/geometry[averageHeight]");

//    InputRef(double, maxRh,"setpoints/humidity/maximumRh[setpoint]");
//    InputRef(double, minDeltaX,"setpoints/humidity/minimumDeltaX[setpoint]");
    Output(double, rh);
    Output(double, rhEq);
    Output(double, ah);
    Output(double, ahEq);
    Output(double, netVapourFlux);
    Output(double, grossVapourFlux);
    Output(double, timeConstant);
    Output(double, extraCondensation);
    Output(double, extraVentilation);
//    Output(double, rhExcess);
//    Output(double, wvpdInsufficiency);
//    Output(bool, highRh);
//    Output(bool, lowDeltaX);
//    Output(bool, highHumidity);
}

void IndoorsHumidity::reset() {
    ah = ahEq = outdoorsAh;
    rh = rhEq = rhFromAh(indoorsTemperature, ah);
    latentHeatBalance = 0.;
}

void IndoorsHumidity::update() {
    double prevAh = ah;
    double gainSum = gain.sum();
    ahEq = gainSum/conductance.sum();
    rhEq = rhFromAh(indoorsTemperature, ahEq);
    timeConstant = averageHeight/gainSum;
    ah = ahEq - (ahEq-ah)*exp(-timeStep/timeConstant);
    netVapourFlux = (ah - prevAh)*averageHeight/timeStep; // kg/m2/s = kg/m3 * m3/m2 / s
    grossVapourFlux = grossFlux.transpiration - grossFlux.condensation - grossFlux.ventilation;

    double surplusAh = ah - sah(indoorsTemperature);
    if (surplusAh > 0.) {
        double  gc, gv;
        double total = grossFlux.condensation+grossFlux.ventilation;
        gc = (total > 0) ? grossFlux.condensation/total : 0;
        gv = 1-gc;
        extraCondensation = gc*surplusAh*averageHeight/timeStep,
        extraVentilation = gv*surplusAh*averageHeight/timeStep;
        ah = sah(indoorsTemperature);
    }
    rh = rhFromAh(indoorsTemperature, ah);
}

void IndoorsHumidity::updateFromMc() {


    //    // Balances
//    rhExcess = rh - maxRh;
//    wvpdInsufficiency = minDeltaX - wvpDeficit;
//    // Flags
//    highRh = rhExcess > 0.;
//    lowDeltaX = wvpdInsufficiency > 0.;
//    highHumidity = highRh || lowDeltaX;
}

} //namespace
