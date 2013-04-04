/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "screen_energy_balance.h"

using namespace UniSim;

namespace vg {
	
ScreenEnergyBalance::ScreenEnergyBalance(Identifier name, QObject *parent)
    : ScreenEnergyBase(name, parent)
{
    new Parameter<double>("globRad", &globRad, 0., this, "Global radiation (W/m2)");
    new Parameter<double>("trGhDir", &trGhDir, 1., this, "Greenhouse direct light transmission [0;1]");
    new Parameter<double>("Tindoors", &Tindoors, 27., this, "Indoors temperature (oC)");
    new Parameter<double>("Toutdoors", &Toutdoors, 15., this, "Outdoors temperature (oC)");
    new Parameter<double>("Kcover", &Kcover, 7.9, this, "K value of greenhouse cover (W/m2/K)");
}

bool ScreenEnergyBalance::useScreen() {
    double energyBalance = trGhDir*globRad - Kcover*(Tindoors - Toutdoors);
    return (energyBalance < 0) && (globRad < spLight);
}

} //namespace
