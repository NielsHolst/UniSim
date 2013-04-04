/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "indoors_humidity.h"

using namespace UniSim;

namespace vg {
	
IndoorsHumidity::IndoorsHumidity(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("Tindoors", &Tindoors, 27., this, "Indoors temperature (oC)");
    new Variable<double>("rh", &rh, this, "Relative humidity (%)");
    new Variable<double>("moistureDeficit", &moistureDeficit, this, "Moisture deficit (g/m3)");
    new Variable<bool>("spRhPassed", &spRhPassed, this, "Is relative humidity above the set point?");
    new Variable<bool>("spDeltaXPassed", &spDeltaXPassed, this, "Is moisture deficit less than the delta x set point?");
    new Variable<bool>("spEitherPassed", &spEitherPassed, this, "Is either set point passed?");
}

void IndoorsHumidity::initialize() {
    Model *sp = seekOneChild<Model*>("sp");
    spRh = sp->pullValuePtr<double>("spRh");
    spDeltaX = sp->pullValuePtr<double>("spDeltaX");
}

void IndoorsHumidity::reset() {
    update();
}

void IndoorsHumidity::update() {
    rh = 85.;
    moistureDeficit = vg::moistureDeficit(Tindoors, rh);
    spRhPassed = (rh > *spRh);
    spDeltaXPassed = (moistureDeficit > *spDeltaX);
    spEitherPassed = spRhPassed || spDeltaXPassed;
}


} //namespace

