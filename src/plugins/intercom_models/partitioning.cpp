/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "partitioning.h"

using namespace UniSim;

namespace intercom{

Partitioning::Partitioning(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    new PullVariable<double>("value", &value, this, "Partioning coefficient");
}

} //namespace

