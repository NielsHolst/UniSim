/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "low_light_switch.h"
#include "publish.h"

using namespace UniSim;

namespace vg {
	
PUBLISH(LowLightSwitch)

/*! \class LowLightSwitch
 * \brief Switches off at low sunlight irradiation
 *
 * Inputs
 * ------
 * - _threshold_ is the sunlight irradiation below which the switch is off [W/m<SUP>2</SUP>]
 * - _outdoorsRadiation_ is the intensity of sunlight irradiation [W/m<SUP>2</SUP>]
 *
 * Outputs
 * ------
 * - _timeOn_ flags whether switch is on or off [true,false]

 * Default dependencies
 * ------------
 * - an _outdoors_ model with a _radiation_ port [W/m<SUP>2</SUP>]
 */

LowLightSwitch::LowLightSwitch(Identifier name, QObject *parent)
    : TimeSwitch(name, parent)
{
    Input(double, threshold, 20.);
    InputRef(double, outdoorsRadiation, "outdoors[radiation]");
    Output(bool, lightOn);
}

void LowLightSwitch::reset() {
    lightOn = false;
}

void LowLightSwitch::update() {
    lightOn = outdoorsRadiation < threshold;
}


} //namespace

