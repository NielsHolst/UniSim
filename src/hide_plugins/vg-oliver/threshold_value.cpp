/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "threshold_value.h"

using namespace UniSim;

namespace vg {

PUBLISH(ThresholdValue)

/*! \class ThresholdValue
 * \brief Sets a value high or low depending a threshold
 *
 * Inputs
 * ------
 * - _testValue_ is the value compared with the threshold value [R]
 * - _thresholdValue_ is the threshold [R]
 * - _lowValue_ is the value when _testValue_ < _testThreshold_ [R]
 * - _highValue_ is the value when _testValue_ >= _testThreshold_ [R]
 *
 * Output
 * ------
 * - _value_ is the value depending on the threshold

 * Default dependencies
 * ------------
 * - none
 */

ThresholdValue::ThresholdValue(Identifier name, QObject *parent)
    : Model(name, parent){
    Input(double, testValue, 0.);
    Input(double, testThreshold, 0.);
    Input(double, lowValue, 0.);
    Input(double, highValue, 0.);
    Output(double, value);
}

void ThresholdValue::reset() {
    value = lowValue;
}

void ThresholdValue::update() {
    value = (testValue < testThreshold) ? lowValue : highValue;
}

} //namespace

