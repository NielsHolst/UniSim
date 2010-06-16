/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "plant.h"

using namespace UniSim;


namespace ambrosia{

Plant::Plant(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
    new PullVariable("stage", &stage, this, "desc");
    new PullVariable("total", &total, this, "desc");
}

void Plant::initialize() {
    setParameter("beginDay", &beginDay, 1, "desc");
    setParameter("beginMonth", &beginMonth, 5, "desc");

    QString bd("bioDays");
    for (int i = 0; i < NumStages; ++i)
        setParameter(bd + ('A' + i) , &bioDays[i], 0., "desc");

    calendar = seekOne<Model*>("calendar");
    timeABDE = seekOneChild<Model*>("timeABDE");
    timeC = seekOneChild<Model*>("timeC");
}

void Plant::reset() {
    stage = total = 0.;
}

void Plant::update() {
    int firstDay = toDayOfYear(beginDay, beginMonth);
    double today = calendar->pullVariable("dayInYear");
    if (today < firstDay)
        return;

    double step = (fabs(stage-3.) < 1e-9) ?
        timeC->pullVariable("step") :
        timeABDE->pullVariable("step");
    total += step;

    double sum = 0;
    for (int i = 0; i < NumStages; ++i) {
        sum += bioDays[i];
        if (total < sum) {
            stage = i + 1;
            return;
        }
    }
    stage = NumStages + 1;;
}

} //namespace

