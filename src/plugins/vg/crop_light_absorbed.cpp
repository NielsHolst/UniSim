/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "general.h"
#include "publish.h"
#include "crop_light_absorbed.h"

using namespace UniSim;

namespace vg {

PUBLISH(CropLightAbsorbed)

CropLightAbsorbed::CropLightAbsorbed(Identifier name, QObject *parent)
	: Model(name, parent)
{
    Input(double, lightAbsorptionCoefficient, 0.66);
    InputRef(double, parAbsorbed, "../photosynthesis[parAbsorbed]");
    Output(double, value);
}

void CropLightAbsorbed::reset() {
    value = 0;
}

void CropLightAbsorbed::update() {
    value = lightAbsorptionCoefficient*parAbsorbed;
}


} //namespace

