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
    addParameterRef<double>(Name(Tgh), "indoors/temperature[air]");
    addParameterRef<double>(Name(rh), "indoors/humidity[rh]");
    addParameter<double>(Name(Tcrop), 27., "Crop temperature (oC)");
    addVariable<double>(Name(Tdew),"Dew point temperature of crop (oC)");
    addVariable<bool>(Name(isWet), "Is crop wet?");
}

void CropWetness::reset() {
    update();
}

void CropWetness::update() {
    Tdew = vg::Tdew(Tcrop, rh);
    isWet = (Tcrop <= Tdew);
}


} //namespace

