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
 * - _netVapourFlux_ is the flux of water vapour in our out of the greenhouse [kg/m<SUP>2</SUP>/s]
 * - _timeConstant_ is the time constant used to integrate _netVapourFlux_ [s<SUP>-1</SUP>]
 * - _surplusAh_ is an integration inaccuracy that was rounded off [kg/m<SUP>3</SUP>]
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

    double indoorsSah = sah(indoorsTemperature);
    if (ah > indoorsSah) {
        ah = indoorsSah;
        surplusAh = ah - indoorsSah;
    }
    else {
        surplusAh = 0.;
    }

    rh = rhFromAh(indoorsTemperature, ah);
    netVapourFlux = (ah - prevAh)*averageHeight/timeStep; // kg/m2/s = kg/m3 * m3/m2 / s
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
