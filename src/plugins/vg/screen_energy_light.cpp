/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_energy_light.h"

using namespace UniSim;

namespace vg {
	
ScreenEnergyLight::ScreenEnergyLight(Identifier name, QObject *parent)
    : ScreenEnergyBase(name, parent)
{
    new Parameter<double>("globRad", &globRad, 0., this, "Global radiation (W/m2)");
}

bool ScreenEnergyLight::useScreen() {
    return (globRad < spLight);
}
} //namespace
