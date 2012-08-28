/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include <usbase/variable.h>
#include "enzymatic_reaction.h"

using namespace UniSim;

namespace MicrobialCommunity {

    EnzymaticReaction::EnzymaticReaction(UniSim::Identifier name, QObject *parent) : Reaction(name, parent) {}

    void EnzymaticReaction::update() {

        double substrateConsumption = 0;
        double substrateConsumptionPDD = 0;

        for(int i = 0; i < populationList.size(); i++) {

            double densityActivityCoef = populationList[i]->pullValue<double>("DensityActivityCoefficient");
            double Km = populationList[i]->pullValue<double>("MichaelisMentenConstant");
            double Vmax = populationList[i]->pullValue<double>("MaximumVelocity");

            velocity = Vmax * substrate / (Km + substrate);
            velocityPDD = densityActivityCoef * velocity;

            substrateConsumption += velocity; // consumption = velocity * 1;
            substrateConsumptionPDD += velocityPDD;
        }

        substrate -=  substrateConsumption;
        substratePDD -=  substrateConsumptionPDD;

        if (substrate <= 0) {
            substrate = 0;
        }

        if (substratePDD <= 0) {
            substratePDD = 0;
        }

    }

} //namespace

