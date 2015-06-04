/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "general.h"
#include "publish.h"
#include "ventilation_by_cooling.h"

using namespace UniSim;

namespace vg {

PUBLISH(VentilationByCooling)

/*! \class VentilationByCooling
 * \brief Total air ventilation
 *
 * Inputs
 * ------
 * - _averageHeight_ is the average height of the greenhouse [m]
 * - _active_ is the active ventilation [h<SUP>-1</SUP>]
 *
 * Outputs
 * ------
 * - _value_ is the relative rate of air exchanged [h<SUP>-1</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _construction/geometry_ model with an _averageHeight_ port [m]
 */

VentilationByCooling::VentilationByCooling(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, averageHeight, "construction/geometry[averageHeight]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, energyFlux,"indoors/active/energyFlux/cooling/supply[state]");
    Output(double, value);
}

void VentilationByCooling::reset() {
    value = 0;
}

void VentilationByCooling::update() {
    double dT = outdoorsTemperature - indoorsTemperature;
    // h-1 = W/m2 / m / K / (J/kg/K) / (kg/m3) * s/h
    value = energyFlux/averageHeight/dT/CpAir/RhoAir*3600;
}

} //namespace

