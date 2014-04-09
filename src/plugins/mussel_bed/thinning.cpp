/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

/* ##MUSSEL POPULATION SELF-THINNING RULE: estimates losses due to increase of individual size
   Thinning. This equation have been calculated based on field observations in the Wadden Sea.
   thinning occur when the density in numbers/m2 is too high for the current average mussel individual
   size. This model evaluates the population after it has grown for the day, and then stablish a maximum
   number of mussel/m2. The difference with the current density numbers is then eliminated from the bed.
   However as starfish predation can have a thinning effect, this thinning value is compared with starfish
   predation for the current date and the bigger one is used to determine the density loss for the mussel bed*/

#include "thinning.h"


using namespace UniSim;

namespace mussel_bed {

thinning::thinning(Identifier name, QObject *parent)
    : Model(name, parent)
{
    new Parameter<double>("Density", &Density, 1., this, "current mussel density biomass g/m2");
    new Parameter<double>("N", &N, 1, this, "current mussel density numbers n/m2");
    new Parameter<double>("supply", &supply, 0., this, "biomass of mussesl consumed by starfish g/m2");
    new Variable<double>("thin", &thin, this, "loss casused by thinning as g/m2");
    new Variable<double>("LossN", &LossN, this, "mussel loss as numbers n/m2");
    new Variable<double>("LossB", &LossB, this, "mussel loss as biomass g/m2");
    new Variable<double>("avgs", &avgs, this, "current mussel mean individual weight");
    new Variable<double>("maxN", &maxN, this, "maximum density in numbers/m2 for the current avgs");
}

void thinning::reset() {
    LossN = 0.;
    LossB = 0.;
    thin = 0.;
    avgs = 0.;
    maxN = 0.;
}

void thinning::update() {
        avgs = (Density/N);
        maxN = 3502*pow(avgs,-0.84);
        thin = ((N-maxN)*avgs);
        if (thin>0 && (thin)>supply){
            LossB = thin;
            LossN = (N-maxN);
        }
        else {
            LossB = supply;
            LossN = supply/avgs;
        }
}
} //namespace

