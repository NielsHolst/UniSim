/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "publish.h"
#include "threshold_switch.h"

using namespace UniSim;

namespace vg {

PUBLISH(ThresholdSwitch)

/*! \class ThresholdSwitch
 * \brief Sets a switch on/off depending on a threshold
 *
 * Inputs
 * ------
 * - _testValue_ is the value compared with the threshold value [R]
 * - _thresholdValue_ is the threshold [R]
 * - _type_ tells whether its is is a high or low threshold  ["high", "low"]
 * - _initValue_ is the initial switch value [true/false]
 *
 * Output
 * ------
 * - _passed_ tells whether the threshold has been passed [true/false]

 * Default dependencies
 * ------------
 * - none
 */

ThresholdSwitch::ThresholdSwitch(Identifier name, QObject *parent)
    : Model(name, parent){
    Input(double, testValue, 0.);
    Input(double, testThreshold, 0.);
    Input(QString, type, "high");
    Input(bool, initValue, false);
    Output(bool, passed);
}

void ThresholdSwitch::initialize() {
    if (type.toLower() == "low")
        testHigh = false;
    else if (type.toLower() == "high")
        testHigh = true;
    else {
        QString msg{"Input 'type' must be either 'low' or 'high' not '%1'"};
        throw Exception(msg.arg(type), this);
    }
}

void ThresholdSwitch::reset() {
    passed = initValue;
}

void ThresholdSwitch::update() {
    passed = (testHigh && testValue > testThreshold) ||
            (!testHigh && testValue < testThreshold);
}

} //namespace

