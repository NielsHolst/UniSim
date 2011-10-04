/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QString>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "insect.h"
#include "insect_life_cycle.h"
#include "stage.h"

namespace UniSim{
	
Insect::Insect(Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Update,ChildrenNot);
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this,
                          "Proportion females");
    new Parameter<double>("eggProduction", &eggProduction, 100., this,
                          "Lifetime egg production of one female");
}

void Insect::initialize() {
    hibernatingAdult = seekOneChild<Stage*>("hibernatingAdult");
    generations = seekChildren<InsectLifeCycle*>("*");
}

void Insect::reset() {
    QList<Stage*> test = generations[0]->stages();
}

void Insect::update() {
    // Transfer hibernating adults from the reproductive stage of the last generation
    double transfer = generations.last()->pullVariable<double>("adultsToHibernation");
    hibernatingAdult->pushVariable<double>("inflow", transfer);
    hibernatingAdult->deepUpdate();

    // Transfer woken hibernating adults to reproductive stage of first generation
    double emergedReproductiveAdults = hibernatingAdult->pullVariable<double>("outflow");
    generations.first()->stages().last()->pushVariable<double>("inflow", emergedReproductiveAdults);

    // And put eggs into the first stage of first generation
    double eggsToLay = reproduction(emergedReproductiveAdults);
    generations.first()->stages().first()->pushVariable<double>("inflow", eggsToLay);

    int n = generations.size();
    for (int gen = 0; gen < n; ++gen ) {
        generations[gen]->update();
        double reproAdults = generations[gen]->pullVariable<double>("eclosedReproductiveAdults");
        double eggsToLay = reproduction(reproAdults);
        if (gen < n-1)
            generations[gen+1]->stages().first()->pushVariable<double>("inflow", eggsToLay);
        else
            Q_ASSERT(eggsToLay==0);
    }
}

double Insect::reproduction(double adults) {
    return adults*sexRatio*eggProduction;
}

} //namespace



