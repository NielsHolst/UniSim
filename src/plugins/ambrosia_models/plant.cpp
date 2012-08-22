/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "plant.h"

using namespace UniSim;


namespace ambrosia{

Plant::Plant(UniSim::Identifier name, QObject *parent)
	: Model(name, parent) {
    new Parameter<int>("beginDay", &beginDay, 1, this, "desc");
    new Parameter<int>("beginMonth", &beginMonth, 5, this, "desc");

    QString bd("bioDays");
    for (int i = 0; i < NumStages; ++i)
        new Parameter<double>(bd + ('A' + i) , &bioDays[i], 0., this, "desc");

    new Variable<double>("stage", &stage, this, "desc");
    new Variable<double>("total", &total, this, "desc");
}

void Plant::initialize() {
    calendar = seekOne<Model*>("calendar");
    timeABDE = seekOneChild<Model*>("timeABDE");
    timeC = seekOneChild<Model*>("timeC");
}

void Plant::reset() {
    stage = total = 0.;
}

void Plant::update() {
    int firstDay = toDayOfYear(beginDay, beginMonth);
    int today = calendar->pullValue<int>("dayOfYear");
    if (today < firstDay)
        return;

    double step = (fabs(stage-3.) < 1e-9) ?
        timeC->pullValue<double>("step") :
        timeABDE->pullValue<double>("step");
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

