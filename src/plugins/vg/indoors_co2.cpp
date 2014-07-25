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
 * - _airExchange_ is the relative exchange of air through ventilation during one time step  [0;1]
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
    InputRef(double, airExchange, "indoors/ventilation[relative]");
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    InputRef(double, averageHeight, "construction/geometry[averageHeight]");
    Input(double, assimilationRate, 0);
    Input(double, injectionRate, 0.);
    Output(double, ppm);
}

void IndoorsCo2::reset() {
    ppm = outdoorsCo2;
    calcDensity();
}

void IndoorsCo2::update() {
    ppm = ppm*(1. - airExchange) + outdoorsCo2*airExchange;
    calcDensity();
}

void IndoorsCo2::calcDensity() {
	density = 0.;
//    density = absFromPpmCo2(ppm)/averageHeight*1000.;  // kg/m3 /m * g/kg
}


} //namespace

