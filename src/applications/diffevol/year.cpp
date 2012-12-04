/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "year.h"

using namespace UniSim;

namespace diffevol {
	
Year::Year(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("fertiliser", &fertiliser, 0., this, "desc");
    new Parameter<double>("fungicide", &fungicide, 0., this, "desc");
    new Parameter<double>("yield", &yield, 0., this, "desc");
}

void Year::reset() {
}

void Year::update() {
}


} //namespace

