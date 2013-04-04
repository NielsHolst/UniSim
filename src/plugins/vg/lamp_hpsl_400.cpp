/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "lamp_hpsl_400.h"

using namespace UniSim;

namespace vg {
	
LampHpsl400::LampHpsl400(Identifier name, QObject *parent)
    : LampBase(name, parent)
{
    new Parameter<bool>("capacityAsInput", &capacityAsInput, false, this, "Capacity given as input?");
}

void LampHpsl400::calculate() {
    const double withBallast = 1.15;
    convectiveHeat = capacity*0.23; // 15% + 8% ballast
    bnLight = capacity*0.42;        // 35% + 7% ballast)
    rnLight = capacity*0.50;
    par = capacity*0.31;   // Slightly improved from the figures in the sources; lamp quality has improved.
    energyUse = capacity*withBallast;

    if (capacityAsInput) {
        convectiveHeat /= withBallast;
        bnLight /= withBallast;
        rnLight /= withBallast;
        par /= withBallast;
        energyUse /= withBallast;
    }
}

} //namespace

