/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREEN_ENERGY_BALANCE_H
#define VG_SCREEN_ENERGY_BALANCE_H

#include "screen_energy_base.h"

namespace vg {

class ScreenEnergyBalance : public ScreenEnergyBase
{
public:
    ScreenEnergyBalance(UniSim::Identifier name, QObject *parent);
private:
    // Parameters
    double globRad, trGhDir, Tindoors, Toutdoors, Kcover;
    // Methods
    bool useScreen();
};
} //namespace


#endif
