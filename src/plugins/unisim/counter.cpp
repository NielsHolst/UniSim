/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "counter.h"
#include "publish.h"

namespace UniSim{

PUBLISH(Counter)

Counter::Counter(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(int, cAmend, 0).from(".../time[step]");
    Input(int, cAmend, 0).reset(0);

    Output(int, cAmend);
    Output(int, cReset).initialize(0);
    Output(int, cUpdate).reset(0);
    Output(int, cCleanup).initialize(0);
    Output(int, cDebrief);
}

void Counter::amend() {
    ++cAmend;
}

void Counter::initialize(){
    ++cInitialize;
}

void Counter::reset() {
    ++cReset;
}

void Counter::update() {
    ++cUpdate;
}

void Counter::cleanup() {
    ++cCleanup;
}

void Counter::debrief() {
    ++cDebrief;
}

} //namespace

