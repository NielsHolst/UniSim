/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "crop_wetness.h"
#include "general.h"
using namespace UniSim;

namespace vg {
	
CropWetness::CropWetness(Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("Tgh", &Tgh, 27., this, "Indoors temperature (oC)");
    new Parameter<double>("Tcrop", &Tcrop, 27., this, "Crop temperature (oC)");
    new Parameter<double>("rh", &rh, 80., this, "Indoors relative humidity (%)");
    new Variable<double>("Tdew", &Tdew, this, "Dew point temperature of crop (oC)");
    new Variable<bool>("isWet", &isWet, this, "Is crop wet?");
}

void CropWetness::reset() {
    update();
}

void CropWetness::update() {
    Tdew = vg::Tdew(Tcrop, rh);
    isWet = (Tcrop <= Tdew);
}


} //namespace

