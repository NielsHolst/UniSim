/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "screen_condensation.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;
using std::max;

namespace vg {

PUBLISH(ScreenCondensation)
/*! \class ScreenCondensation
 * \brief Condensation on greenhouse screens
 *
 * Inputs
 * ------
 * - _screenArea_ is the area of drawn screens per greenhouse area [m<SUP>2</SUP>/m<SUP>2</SUP>]
 * - _indoorsTemperature_ is the ambient temperature indoors [<SUP>o</SUP>C]
 * - _indoorsAh_ is the indoors absolute humidity [kg/m<SUP>3</SUP>]
 * - _screenTemperature_ is the temperature of the greenhouse screens [<SUP>o</SUP>C]
 *
 * Outputs
 * ------
 * - see VapourFlux
 *
 * Default dependencies
 * ------------
 * - an _indoors/screens/transmission_ model with an _area_ port
 * - an _indoors/screens/temperature_ model with a _value_ port
 * - an _indoors/temperature_ model with a _value_ port
 * - an _indoors/humidity_ model with an _ah_ port
 */

ScreenCondensation::ScreenCondensation(Identifier name, QObject *parent)
    : VapourFlux(name, parent)
{
    InputRef(double, screenArea, "indoors/screens/transmission[area]");
    InputRef(double, screenTemperature, "indoors/screens/temperature[value]");
    InputRef(double, indoorsTemperature, "indoors/temperature[value]");
    InputRef(double, indoorsAh, "indoors/humidity[ah]");
}

void ScreenCondensation::update() {
    double dTemp = indoorsTemperature - screenTemperature;
    conductance = dTemp > 0
                  ? screenArea*1.64e-3*pow(dTemp, 1/3.) // GCC, p.147
                  : 0.;
    double screenSah = sah(screenTemperature);
    vapourFlux = max(conductance*(indoorsAh - screenSah), 0.);
    gain = conductance*screenSah;
}

} //namespace

