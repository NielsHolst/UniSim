/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include <usbase/exception.h>
#include <usbase/object_pool.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "random_generator.h"

namespace UniSim{

RandomGenerator::RandomGenerator()
    : _generator(0)
{
}

RandomGenerator::~RandomGenerator() {
    delete _generator;
}

RandomGenerator::Generator* RandomGenerator::generator() {
    if (!_generator)
        _generator = new Generator;
    return _generator;
}

} // namespace
