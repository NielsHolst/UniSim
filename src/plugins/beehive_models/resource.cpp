/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "resource.h"

using namespace UniSim;


namespace beehive {

Resource::Resource(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
	new Parameter<double>("Ninit", &Ninit, 1., this, "Description");
	new Parameter<double>("K", &K, 1000., this, "Description");
	new Parameter<double>("r", &r, 1.2, this, "Description");
	new PullVariable<double>("N", &density, this, "Description");
}

void Resource::reset() {
	density = Ninit;
}

void Resource::update() {
	density += (K<=0) ? 0 : density*r*(K-density)/K;
}

} //namespace

