/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_co2.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(IndoorsCo2)

/*! \class IndoorsCo2
 * \brief Keeps track of indoors CO2 concentraion
 *
 * Inputs
 * ------
 * - _outdoorsCo2_ is the CO<SUB>2</SUB> concentration outside the greenhouse [ppm]
 * - _ventilationRate_ is ventilation rate[m<SUP></SUP>/m<SUP>2</SUP>/s]
 * - _assimilationRate_ is the rate of CO<SUB>2</SUB> uptake by the plants [g/m<SUP>2</SUP>/h]
 * - _injectionRate_ is the rate of CO<SUB>2</SUB> injection [g/m<SUP>2</SUP>/h]
 * - _timeStep_ is the duration of one integration time step [2]
 * - _averageHeight_ is the average height of the greenhouse [m]
 *
 * Output
 * ------
 * - _ppm_ is the CO<SUB>2</SUB> concentration inside the greenhouse [ppm]
 * - _density_ is the density of CO<SUB>2</SUB> per greenhouse area [g/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - an _outdoors_ model with a _co2_ port [ppm]
 * - an _indoors/ventilation_ model with a _relative_ port [0;1]
 * - an _construction/geometry_ model with an _averageHeight_ port [m]
 */

IndoorsCo2::IndoorsCo2(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, outdoorsCo2, "outdoors[co2]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
//    InputRef(double, ventilationRate, "indoors/ventilation[rate]");
    InputRef(double, averageHeight, "construction/geometry[averageHeight]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    Input(double, assimilationRate, 0);
    Input(double, injectionRate, 0.);
    Output(double, ppm);
    Output(double, density);
	Output(double, ventilationLoss);
}

void IndoorsCo2::reset() {
    ppm = outdoorsCo2;
	density = ppmToDensity(ppm);
	ventilationLoss = 0.;
}

void IndoorsCo2::update() {
    /*
    double ppmBefore = ppm,
        finiteExchangerate = 1 - exp(-ventilationRate*timeStep);
    ppm = ppm*(1. - finiteExchangerate) + outdoorsCo2*finiteExchangerate;
	ventilationLoss = ppmToDensity(ppmBefore-ppm);
	ppm += (injectionRate - assimilationRate)*timeStep/3600;
	density = ppmToDensity(ppm);
    */
}

double IndoorsCo2::ppmToDensity(double ppm) {
	return absFromPpmCo2(indoorsTemperature, ppm)*averageHeight*1000.;  // g/m2 = kg/m3 * m * g/kg
}

} //namespace
