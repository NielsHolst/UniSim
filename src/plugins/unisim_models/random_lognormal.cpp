/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "random_lognormal.h"

namespace UniSim{

RandomLognormal::RandomLognormal(Identifier name, QObject *parent)
    : RandomBase(name, parent)
{
    generator = new Generator;
    distribution = new Distribution(minValue, maxValue);
    variate = new Variate(*generator, *distribution);
}

RandomLognormal::~RandomLognormal() {
    delete generator;
    delete distribution;
    delete variate;
}

void RandomLognormal::update() {
    value = (*variate)();
}

} //namespace
