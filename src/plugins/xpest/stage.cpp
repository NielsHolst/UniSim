/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "day_degree_scale.h"
#include "publish.h"
#include "stage.h"

using namespace UniSim;

namespace xpest {

PUBLISH(Stage)

Stage::Stage(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Output(double, growthStage); // Growth stage value within this stage
}

void Stage::amend() {
    QList<DayDegreeScale*> dd = seekChildren<DayDegreeScale*>("*");
    for (int i = 0; i < dd.size(); ++i) {
        ddScales << dd.at(i);
    }
}

void Stage::reset() {
    growthStage = ddScales.at(0)->pullValue<double>("from");
}

void Stage::update() {
    int i = 0;
    while (i < ddScales.size() - 1 &&
           ddScales.at(i)->pullValue<bool>("completed")) ++i;
    growthStage = ddScales.at(i)->pullValue<double>("growthStage");
}

} //namespace

