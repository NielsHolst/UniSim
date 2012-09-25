/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include "hive.h"

using namespace UniSim;


namespace beehive {

Hive::Hive(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("x", &x, 0., this, "Geographical x-coordinate");
    new Parameter<double>("y", &y, 0., this, "Geographical y-coordinate");
}


} //namespace

