/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "random_lognormal.h"

namespace UniSim{

RandomLognormal::RandomLognormal(Identifier name, QObject *parent)
    : RandomBase(name, parent), distribution(0), variate(0)
{
    new Parameter<double>("mean", &mean, 10., this, "Mean of log-normal distribution");
    new Parameter<double>("sd", &sd, 2., this, "Standard deviation of log-normal distribution");
}

RandomLognormal::~RandomLognormal() {
    delete distribution;
    delete variate;
}

void RandomLognormal::initialize() {
    RandomBase::initialize();
    distribution = new Distribution(mean, sd);
    variate = new Variate(*randomGenerator(), *distribution);
}

double RandomLognormal::drawValue() {
    return (*variate)();
}

} //namespace
