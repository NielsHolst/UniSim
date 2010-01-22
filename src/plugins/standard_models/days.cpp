/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>
#include "days.h"

namespace UniSim{

Days::Days(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setState("step", &step);
    setState("total", &total);
}

void Days::reset() {
    step = 1.;
    total = 0.;
}

void Days::update()
{
    total += step;
}

} //namespace

