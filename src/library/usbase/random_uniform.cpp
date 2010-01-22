/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "random_uniform.h"

namespace UniSim{
	
RandomUniform* RandomUniform::_randomUniform = 0;

RandomUniform::RandomUniform()
    : uni_dist(0,1), nextVariate(generator, uni_dist)
{
}

QString RandomUniform::id() {
    return "RandomUniform";
}

void RandomUniform::setSeed(unsigned seed) {
    generator.seed(seed);
}

double RandomUniform::next() {
    return nextVariate();
}

} //namespace

