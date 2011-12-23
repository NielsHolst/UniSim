/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "pt_immigration.h"

using namespace UniSim;

namespace grainstore{

PtImmigration::PtImmigration(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("initial", &initial, 10., this, "desc");
    new Parameter<double>("continuous", &continuous, 0., this, "desc");
    new PullVariable<double>("current", &current, this, "desc");
}

void PtImmigration::initialize()
{
}

void PtImmigration::reset()
{
    useInitial = true;
}

void PtImmigration::update()
{
    current = useInitial ? initial : continuous;
    useInitial = false;
}

} //namespace

