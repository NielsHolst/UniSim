/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "plant.h"

using namespace UniSim;


namespace ambrosia{

Plant::Plant(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
    setState("stage", &stage);
    setState("total", &total);
}

void Plant::initialize() {
    setParameter("beginDay", &beginDay, 1);
    setParameter("beginMonth", &beginMonth, 5);

    QString bd("bioDays");
    for (int i = 0; i < NumStages; ++i)
        setParameter(bd + ('A' + i) , &bioDays[i], 0.);

    calendar = seekOne<Model*>("calendar");
    timeABDE = seekOneChild<Model*>("timeABDE");
    timeC = seekOneChild<Model*>("timeC");
}

void Plant::reset() {
    stage = total = 0.;
}

void Plant::update() {
    int firstDay = toDayOfYear(beginDay, beginMonth);
    double today = calendar->state("dayInYear");
    if (today < firstDay)
        return;

    double step = (fabs(stage-3.) < 1e-9) ?
        timeC->state("step") :
        timeABDE->state("step");
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

