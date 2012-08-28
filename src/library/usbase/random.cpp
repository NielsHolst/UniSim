/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "random.h"

namespace UniSim{

Random *Random::theRandomGenerator = 0;

Random::Random()
{
}

QString Random::id() {
    return "Random";
}

} // namespace
