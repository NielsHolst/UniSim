/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "plot.h"

using namespace UniSim;

namespace diffevol {
	
Plot::Plot(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("area", &area, 1., this, "desc");
    new Parameter<double>("slope", &area, 10., this, "desc");
}

void Plot::reset() {
}

void Plot::update() {
}


} //namespace

