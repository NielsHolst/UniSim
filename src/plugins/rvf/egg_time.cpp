/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "egg_time.h"
#include "publish.h"

using namespace UniSim;

namespace rvf {

PUBLISH(EggTime)

EggTime::EggTime(Identifier name, QObject *parent)
     : Model(name, parent)
{
    Input(double, waterLevel, 0.);
    Input(double, waterLevelThreshold, 0.);
    InputRef(double, daydegrees, "./daydegrees[step]");
    Output(double, step);
    Output(double, total);
}

void EggTime::reset() {
    step = total = 0.;
}

void EggTime::update() {
    step = (waterLevel < waterLevelThreshold) ? 0. : daydegrees;
    total += step;
}

} //namespace
