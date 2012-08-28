/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <QString>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "inachis.h"
#include "insect_life_cycle.h"
#include "stage.h"

namespace UniSim{
	
Inachis::Inachis(Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Update,ChildrenNot);
    new Parameter<double>("sexRatio", &sexRatio, 0.5, this,
                          "Proportion females");
    new Parameter<double>("eggProduction", &eggProduction, 100., this,
                          "Lifetime egg production of one female");
}

void Inachis::initialize() {
    hibernatingAdult = seekOneChild<Stage*>("hibernatingAdult");
    generations = seekChildren<InsectLifeCycle*>("*");
}

void Inachis::reset() {
    QList<Stage*> test = generations[0]->stages();
}

void Inachis::update() {
    // Transfer hibernating adults from the reproductive stage of the last generation
    double transfer = generations.last()->pullValue<double>("adultsToHibernation");
    hibernatingAdult->pushValue<double>("inflow", transfer);
    hibernatingAdult->deepUpdate();

    // Transfer woken hibernating adults to reproductive stage of first generation
    double emergedReproductiveAdults = hibernatingAdult->pullValue<double>("outflow");
    generations.first()->stages().last()->pushValue<double>("inflow", emergedReproductiveAdults);

    // And put eggs into the first stage of first generation
    double eggsToLay = reproduction(emergedReproductiveAdults);
    generations.first()->stages().first()->pushValue<double>("inflow", eggsToLay);

    int n = generations.size();
    for (int gen = 0; gen < n; ++gen ) {
        generations[gen]->update();
        double reproAdults = generations[gen]->pullValue<double>("eclosedReproductiveAdults");
        double eggsToLay = reproduction(reproAdults);
        if (gen < n-1)
            generations[gen+1]->stages().first()->pushValue<double>("inflow", eggsToLay);
        else
            Q_ASSERT(eggsToLay==0);
    }
}

double Inachis::reproduction(double adults) {
    return adults*sexRatio*eggProduction;
}

} //namespace



