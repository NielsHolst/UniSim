/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/test_num.h>
#include <usbase/variable.h>
#include "resource.h"

using namespace UniSim;


namespace beehive {

Resource::Resource(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("initial", &initial, 100., this, "Initial amount of resource");
    new Parameter<double>("decrement", &decrement, 1., this, "Decrement in resource in next time step");
    new Parameter<double>("zeroTolerance", &zeroTolerance, 1e-6, this, "Round resource to zero within this tolerance");
    new Variable<double>("value", &value, this, "Current amount of resource");
}

void Resource::reset() {
    value = initial;
}

void Resource::update() {
    value -= decrement;
    TestNum::snapToZero(value, zeroTolerance);
}

} //namespace

