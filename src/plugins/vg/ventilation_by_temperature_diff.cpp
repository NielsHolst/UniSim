/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "general.h"
#include "publish.h"
#include "ventilation_by_temperature_diff.h"
#include <usbase/utilities.h>

using namespace std;
using namespace UniSim;

namespace vg {
	
PUBLISH(VentilationByTemperatureDiff)

/* \class VentilationByTemperatureDiff
 * \brief Check this! Calculates ventilation opening from temperature diffence to outdoors
 *
 * Inputs
 * ------
 * - _outdoorsTemperature_ is the ambient temperature outdoors [<SUP>o</SUP>C]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _ventilationMax_ is the maximum ventilation opening [0;100]
 *
 * Outputs
 * ------
 * - _signal_ is the desired ventilation opening [0;100]

 * Dependencies
 * ------------
 * - an _outdoors_ model with a _temperature_ port [<SUP>o</SUP>C]
 * - an _indoors/temperature_ model with a _value_ port [<SUP>o</SUP>C]
 * - a _maximum_ sibling model with a _signal_ port [0;100]
 */

VentilationByTemperatureDiff::VentilationByTemperatureDiff(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, outdoorsTemperature, "outdoors[temperature]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, ventilationMax, "../maximum[signal]");
    Output(double, signal);
}

void VentilationByTemperatureDiff::reset() {
    signal = 0;
}

void VentilationByTemperatureDiff::update() {
    double diff = indoorsTemperature - outdoorsTemperature;
    signal = max(ventilationMax - 1./(0.75*exp(-diff/10)), 0.);
}

} //namespace

