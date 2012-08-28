/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "random_uniform.h"

namespace UniSim{

RandomUniform::RandomUniform(Identifier name, QObject *parent)
    : RandomBase(name, parent), distribution(0), variate(0)
{
}

RandomUniform::~RandomUniform() {
    delete distribution;
    delete variate;
}

void RandomUniform::initialize() {
    RandomBase::initialize();
    distribution = new Distribution(pullValue<double>("minValue"), pullValue<double>("maxValue"));
    variate = new Variate(*randomGenerator(), *distribution);
}

double RandomUniform::drawValue() {
    return (*variate)();
}

} //namespace
