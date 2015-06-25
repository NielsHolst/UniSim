/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "publish.h"
#include "screen_air_transmission.h"

using namespace UniSim;
using std::min;

namespace vg {

PUBLISH(ScreenAirTransmission)

/*! \class ScreenAirTransmission
 * \brief The obligatory air flux is leakage plus humidity-controlled ventilation
 *
 * Inputs
 * ------
 * - _airTransmission_ is the proportion of air led through the screens [0;1]
 *
 * Output
 * ------
 * - _value_ is the transmission of air through screen and screen gap [0;1]
 */

ScreenAirTransmission::ScreenAirTransmission(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, state, 0.);
    Input(double, airTransmission, 1.);
    Input(double, exponent, 4.);
    Output(double, transmitted);
    Output(double, notTransmitted);
}

void ScreenAirTransmission::reset() {
    notTransmitted = 0;
    transmitted = 1.;
}

void ScreenAirTransmission::update() {
    transmitted = min( pow(1. - state, exponent) + airTransmission, 1. );
    notTransmitted = 1. - transmitted;
}


} //namespace

