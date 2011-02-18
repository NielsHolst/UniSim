/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "partitioning_fixed.h"

using namespace UniSim;

namespace intercom{

PartitioningFixed::PartitioningFixed(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("value", &value, 1., this,
    "Fixed partitioning coefficient. They must sum up to one for a @F Plant object.");

    new PullVariable<double>("value", &value, this, "Partioning coefficient");
}

} //namespace

