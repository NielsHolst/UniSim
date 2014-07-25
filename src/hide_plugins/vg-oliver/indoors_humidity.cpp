/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_humidity.h"
#include "publish.h"
#include "vapour_flux.h"

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
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsAh, "outdoors[ah]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, averageHeight, "construction/geometry[averageHeight]");

    Output(double, rh);
    Output(double, ah);
    Output(double, ahEq);
    Output(double, netVapourFlux);
    Output(double, grossVapourFlux);
    Output(double, timeConstant);
    Output(double, surplusAh);
}

void IndoorsHumidity::reset() {
    ah = ahEq = outdoorsAh;
    rh = rhFromAh(indoorsTemperature, ah);
}

void IndoorsHumidity::initialize() {
    all.clear();
    all << &transpiration
        << &evaporation
        << &condensation
        << &ventilation;
    collectFluxes();
}

void IndoorsHumidity::update() {
    double prevAh = ah,
           gainSum = sumGain(),
           conductanceSum = sumConductance();
    ahEq = conductanceSum>0 ? gainSum/conductanceSum : 0.;
    timeConstant = averageHeight/gainSum;
    ah = ahEq - (ahEq-ah)*exp(-timeStep/timeConstant);

    netVapourFlux = (ah - prevAh)*averageHeight/timeStep; // kg/m2/s = kg/m3 * m3/m2 / s
    grossVapourFlux = sumVapourFlux(transpiration)
                    + sumVapourFlux(evaporation)
                    - sumVapourFlux(condensation)
                    - sumVapourFlux(ventilation);

    double indoorsSah = sah(indoorsTemperature);
    surplusAh = max(ah-indoorsSah, 0.);
    ah = min(ah, indoorsSah);
    rh = rhFromAh(indoorsTemperature, ah);

//    if (surplusAh > 0.) {
//        double  gc, gv;
//        double total = grossFlux.condensation + grossFlux.ventilation;
//        gv = (total>0 &&) ? grossFlux.condensation/total : 0;
//        gv = 1-gc;
//        extraCondensation = gc*surplusAh*averageHeight/timeStep,
//        extraVentilation = gv*surplusAh*averageHeight/timeStep;
//        ah = sah(indoorsTemperature);
//    }
}

void IndoorsHumidity::collectFluxes() {
    transpiration = collectFluxes("transpiration");
    evaporation = collectFluxes("evaporation");
    condensation = collectFluxes("condensation");
    ventilation = collectFluxes("ventilation");
}

IndoorsHumidity::Fluxes IndoorsHumidity::collectFluxes(QString fluxName) {
    auto child = peekOneChild<Model*>(fluxName);
    auto fluxes = child->seekChildren<VapourFlux*>("*");
    QVector<FluxPtr> ptrs;
    for (auto flux : fluxes) {
        FluxPtr p;
        p.conductance = flux->pullValuePtr<double>("conductance");
        p.gain = flux->pullValuePtr<double>("gain");
        p.vapourFlux = flux->pullValuePtr<double>("vapourFlux");
        ptrs << p;
    }
    return ptrs;
}

double IndoorsHumidity::sumConductance() {
    double sum{0};
    for (auto fluxes : all)
        for (auto flux : *fluxes)
            sum += *flux.conductance;
    return sum;
}

double IndoorsHumidity::sumGain() {
    double sum{0};
    for (auto fluxes : all)
        for (auto flux : *fluxes)
            sum += *flux.gain;
    return sum;
}

double IndoorsHumidity::sumVapourFlux(const IndoorsHumidity::Fluxes &fluxes) {
    double sum{0};
    for (auto flux : fluxes)
        sum += *flux.vapourFlux;
    return sum;
}
} //namespace
