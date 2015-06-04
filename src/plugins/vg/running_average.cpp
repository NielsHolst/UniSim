/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "running_average.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(RunningAverage)

/*! \class RunningAverage
 * \brief Computes the running average of the input
 *
 * Inputs
 * ------
 * - _initialValue_ is the value assigned to _value_ at reset [R]
 * - _value_ will be put into the running average on next call of update [R]
 * - _length_ is the number of updates the average should run over [R]
 *
 * Output
 * ------
 * - _value_ is the running average [R]
 *
 */

RunningAverage::RunningAverage(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, initialValue, 0.);
    Input(double, input, 0.);
    Input(int, length, 10);
    Output(double, value);
}

void RunningAverage::reset() {
    buffer.fill(initialValue, length);
    sum = initialValue*length;
    value = initialValue;
    ix = 0;
}

void RunningAverage::update() {
    sum += input - buffer.at(ix);
    buffer[ix] = input;
    value = sum/length;
    ix = (ix + 1) % length;
}


} //namespace

