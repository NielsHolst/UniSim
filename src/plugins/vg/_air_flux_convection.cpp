/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "publish.h"
#include "air_flux_convection.h"

using namespace UniSim;
using std::min;

namespace vg {

PUBLISH(AirFluxConvection)

/*! \class AirFluxConvection
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

AirFluxConvection::AirFluxConvection(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, state, 0.);
    Input(double, airTransmission, 1.);
    Input(double, exponent, 4.);
    InputRef(double, volumeBelowRoof, "construction/geometry[volumeBelowRoof]");
    InputRef(double, volumeRoof, "construction/geometry[volumeRoof]");
    Output(double, downwards);
    Output(double, upwards);
}

void AirFluxConvection::reset() {
    // The bottom part of the greenhouse cannot get a larger share than its
    // proportion of the total greenhouse volume
    bottomProp = volumeBelowRoof/(volumeRoof+volumeBelowRoof);
    downwards = bottomProp;
    upwards = 1. - downwards;
}

void AirFluxConvection::update() {
    downwards = min( pow(1. - state, exponent) + airTransmission, bottomProp );
    upwards = 1. - downwards;
}


} //namespace

