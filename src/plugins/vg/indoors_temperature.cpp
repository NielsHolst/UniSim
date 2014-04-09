/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "indoors_temperature.h"

using namespace UniSim;

namespace vg {
	
IndoorsTemperature::IndoorsTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    addParameterRef<double>(Name(value),"outdoors[temperature]");
}


void IndoorsTemperature::reset() {
}

void IndoorsTemperature::update() {
}

} //namespace
