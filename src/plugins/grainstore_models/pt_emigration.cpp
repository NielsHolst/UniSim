/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/pull_variable.h>
#include "pt_emigration.h"

using namespace UniSim;

namespace grainstore{

PtEmigration::PtEmigration(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new PullVariable<double>("N", &density, this, "desc");
}

void PtEmigration::initialize()
{
    setParameter("Ninit", &Ninit, 1., "desc");
    setParameter("K", &K, 1000., "desc");
    setParameter("r", &r, 1.2, "desc");
}

void PtEmigration::reset()
{
	density = Ninit;
}

void PtEmigration::update()
{
	density += (K<=0) ? 0 : density*r*(K-density)/K;
}

} //namespace

