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
    Output(double, lightTransmission);
    Output(double, airTransmission);
}

void Screens::initialize() {
    auto screens = seekChildren<Model*>("*");
    screenInfos.clear();
    for (auto screen: screens) {
        screenInfos << ScreenInfo {
            screen->pullValuePtr<double>("state"),
            screen->pullValuePtr<double>("lightTransmission"),
            screen->pullValuePtr<double>("airTransmission")
        };
    }
}

void Screens::reset() {
    maxState = 0;
    lightTransmission = airTransmission = 1;
}

void Screens::update() {
    maxState = 0;
    lightTransmission = airTransmission = 1;
    for (auto info: screenInfos) {
        maxState = max(maxState, *info.state);
        lightTransmission *= (*info.lightTransmission);
        airTransmission *= (*info.airTransmission);
    }
}



} //namespace

