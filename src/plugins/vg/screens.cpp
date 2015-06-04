/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <usbase/exception.h>
#include "publish.h"
#include "screens.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(Screens)


/*! \class Screens
 * \brief A collection of Screen objects
 *
 * Outputs
 * ------
 * - _maxState_ is the maximum value of the screen states [0;1]
 */

Screens::Screens(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Output(double, maxState);
}

void Screens::initialize() {
    auto screens = seekChildren<Model*>("*");
    states.clear();
    for (auto screen : screens) {
        states << screen->pullValuePtr<double>("state");
    }
}

void Screens::reset() {
    maxState = 0;
}

void Screens::update() {
    maxState = 0;
    for (auto state : states) {
        maxState = max(maxState, *state);
    }
}



} //namespace

