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
    Input(double, airTransmissionExponent, 4.);
    Output(double, maxState);
    Output(double, lightTransmission);
    Output(double, airTransmission);
    Output(double, airTransmissionNot);
    Output(double, gap);
    Output(double, haze);
    Output(double, U);
}

void Screens::initialize() {
    auto screens = seekChildren<Model*>("*");
    screenInfos.clear();
    for (auto screen: screens) {
        screenInfos << ScreenInfo {
            screen->pullValuePtr<double>("state"),
            screen->pullValuePtr<double>("lightTransmissivity"),
            screen->pullValuePtr<double>("haze"),
            screen->pullValuePtr<double>("airTransmission"),
            screen->pullValuePtr<double>("U")
        };
    }
}

void Screens::reset() {
    maxState = airTransmissionNot = 0;
    lightTransmission = airTransmission = 1;
    U = numeric_limits<double>::infinity();
}

void Screens::update() {
    maxState = 0;
    lightTransmission = airTransmission = 1;
    double resistance{0}, unhazed{1};
    for (auto info: screenInfos) {
        maxState = max(maxState, *info.state);
        lightTransmission *= info.lightTransmissionTotal();
        airTransmission *= info.airTransmissionTotal(airTransmissionExponent);
        unhazed *= info.unhazed();
        resistance += info.resistance();
    }
    airTransmissionNot = 1. - airTransmission;
    gap = pow(1. - maxState, airTransmissionExponent);
    haze = 1. - unhazed;
    U = 1./resistance;
}



} //namespace

