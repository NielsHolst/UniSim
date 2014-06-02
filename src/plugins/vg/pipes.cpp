/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "pipes.h"
#include "publish.h"
using namespace UniSim;

namespace vg {

PUBLISH(Pipes)

/*! \class Pipes
 * \brief Temperature and effect of all pipes
 *
 * Inputs
 * ------
 * - _timeStep_ is the integration time step [s]
 *
 * Outputs
 * ------
 * - _effect_ is the energy emission per greenhouse area [W/m<SUP>2</SUP>]
 * - _sumEnergy_ is the summed energy spent per greenhouse area [MJ/m<SUP>2</SUP>]
 *
 * Default dependencies
 * ------------
 * - a _calendar_ model with a _timeStepSecs_ port [s]
 *
 * Optional dependencies
 * ------------
 * - some (0..n) child models with an _effect_ port [W/m<SUP>2</SUP>]
 */

Pipes::Pipes(Identifier name, QObject *parent)
	: Model(name, parent)
{
    InputRef(double, timeStep, "calendar[timeStepSecs]");
    Output(double, effect);
    Output(double, sumEnergy);
}

void Pipes::initialize() {
    pipeEffects.clear();
    auto pipes = seekChildren<Model*>("*");
    for (auto pipe : pipes) {
        auto effect = pipe->peekValuePtr<double>("effect");
        if (effect) pipeEffects << effect;
    }
}

void Pipes::reset() {
    effect = sumEnergy = 0;
}

void Pipes::update() {
    effect = 0;
    for (auto eff : pipeEffects) effect += *eff;
    sumEnergy += effect*timeStep*1e-6;
}

} //namespace

