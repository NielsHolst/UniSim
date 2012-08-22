/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/utilities.h>
#include "../unisim_models/stage.h"
#include "insect.h"

using namespace UniSim;

namespace cotton{

Insect::Insect(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<QDate>("initEgglayingDate", &initEgglayingDate, QDate(2009,3,1), this, "Initial egglaying date");
    new Parameter<double>("initEggs", &initEggs, 30., this, "Initial number of insect eggs");
    new Variable<bool>("eggsLaid", &eggsLaid, this, "Have eggs been laid?");
    setRecursionPolicy(Component::Update, Component::ChildrenNot);
}

void Insect::initialize() {
    QList<Model*> runIterators = seekMany<Model*>("runIterator");
    if (runIterators.size() == 0)
        runIterator = 0;
    else if (runIterators.size() == 1)
        runIterator = runIterators[0];
    else
        throw Exception("At most one 'RunIterator' model is allowed");

    calendar = seekOne<Model*>("calendar");
    egg = seekOneChild<Model*>("egg");
    stages = seekChildren<Stage*>("*");
}

void Insect::reset() {
    eggsLaid = false;
}

void Insect::update() {
    // Lay eggs
    if (isEgglayingDate())
        egg->pushValue("inflow", initEggs);

    // Development of life stages
    stages[0]->deepUpdate();
    for (int i = 1; i < stages.size(); ++i) {
        stages[i]->pushValue("inflow", stages[i-1]->pullValue<double>("outflow"));
        stages[i]->deepUpdate();
    }
}

bool Insect::isEgglayingDate() {
    QDate today = calendar->pullValue<QDate>("date");
    int iteration = runIterator ? runIterator->pullValue<int>("iteration") : 1;
    QDate egglayingDate = initEgglayingDate.addDays(iteration - 1);

    eggsLaid = (today == egglayingDate);
    return eggsLaid;
}

} //namespace

