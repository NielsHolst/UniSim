/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "high_light_switch.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(HighLightSwitch)

/*! \class HighLightSwitch
 * \brief Switches off at high sunlight irradiation
 *
 * Inputs
 * ------
 * - _threshold_ is the sunlight irradiation above which the switch is off [W/m<SUP>2</SUP>]
 * - _outdoorsRadiation_ is the intensity of sunlight irradiation [W/m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _lightOff_ flags whether switch is on or off [true,false]

 * Default dependencies
 * ------------
 * - an _outdoors_ model with a _radiation_ port [W/m<SUP>2</SUP>]
 */

HighLightSwitch::HighLightSwitch(Identifier name, QObject *parent)
    : TimeSwitch(name, parent)
{
    Input(double, threshold, 40.);
    InputRef(double, outdoorsRadiation, "outdoors[radiation]");
    Output(bool, lightOff);
}

void HighLightSwitch::reset() {
    lightOff = false;
}

void HighLightSwitch::update() {
    lightOff = outdoorsRadiation > threshold;
}


} //namespace

