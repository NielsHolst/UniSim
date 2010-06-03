/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
    new PullVariable("current", &current, this);
}

void PtImmigration::initialize()
{
    setParameter("initial", &initial, 10.);
    setParameter("continuous", &continuous, 0.);
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

