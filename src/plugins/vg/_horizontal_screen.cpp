/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include "publish.h"
#include "horizontal_screen.h"

using namespace UniSim;
using std::max;
using std::min;

namespace vg {

PUBLISH(HorizontalScreen)

/*! \class HorizontalScreen
 * \brief The obligatory air flux is leakage plus humidity-controlled ventilation
 *
 * Inputs
 * ------
 *
 * Output
 * ------
 * - _gap_ is the transmission of air through screen gap [0;1]
 */

HorizontalScreen::HorizontalScreen(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, state, "./screens[maxState]");
    InputRef(double, airTransmission, "./screens[airTransmission]");
    Input(double, exponent, 4.);
    Output(double, gap);
    Output(double, notGap);
}

void HorizontalScreen::reset() {
    gap = 1.;
    notGap = 0.;
}

void HorizontalScreen::update() {
    gap = max(pow(1. - state, exponent), airTransmission);
    gap = min(gap, 1.);
    notGap = 1. - gap;
}


} //namespace

