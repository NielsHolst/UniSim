/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "publish.h"
#include "random_poisson.h"

namespace UniSim{

PUBLISH(RandomPoisson)

RandomPoisson::RandomPoisson(Identifier name, QObject *parent)
    : RandomBase(name, parent), distribution(0), variate(0)
{
    new Parameter<double>("mean", &mean, 10., this, "Mean of Poisson distribution");
}

RandomPoisson::~RandomPoisson() {
    delete distribution;
    delete variate;
}

void RandomPoisson::initialize() {
    RandomBase::initialize();
    distribution = new Distribution(mean);
    variate = new Variate(*randomGenerator(), *distribution);
}

double RandomPoisson::drawValue() {
    return (*variate)();
}

} //namespace
