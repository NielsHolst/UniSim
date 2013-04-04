/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_temperature.h"

using namespace UniSim;

namespace vg {
	
IndoorsTemperature::IndoorsTemperature(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("rh", &rh, 80., this, "Indoors humidity (%)");
    new Variable<double>("air", &air, this, "Air temperature (oC)");
    new Variable<double>("dewPoint", &dewPoint, this, "Dew point temperaure (oC)");
}

void IndoorsTemperature::reset() {
    air = 27;
}

void IndoorsTemperature::update() {
    dewPoint = Tdew(air, rh);
}


} //namespace

