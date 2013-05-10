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
    addParameterRef<double>(Name(globRad), "outdoors/records[globRad]");
    addParameterRef<double>(Name(trGhDir), "greenhouse/transmission[direct]");
    addParameterRef<double>(Name(Tindoors), "indoors/temperature[air]");
    addParameterRef<double>(Name(Toutdoors), "outdoors/records[Tair]");
    addParameterRef<double>(Name(Kcover), "greenhouse[Kcover]");
}

bool ScreenEnergyBalance::useScreen() {
    double energyBalance = trGhDir*globRad - Kcover*(Tindoors - Toutdoors);
    return (energyBalance < 0) && (globRad < spLight);
}

} //namespace
