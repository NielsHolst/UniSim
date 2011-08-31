/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "random_generator.h"

namespace UniSim{

RandomGenerator *RandomGenerator::theRandomGenerator = 0;

RandomGenerator::RandomGenerator()
    : _generator(0)
{
}

RandomGenerator::~RandomGenerator() {
    delete _generator;
}

QString RandomGenerator::id() {
    return "RandomGenerator";
}

RandomGenerator::Generator* RandomGenerator::generator() {
    if (!_generator)
        _generator = new Generator;
    return _generator;
}

} // namespace
