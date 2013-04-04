/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREEN_ENERGY_LIGHT_H
#define VG_SCREEN_ENERGY_LIGHT_H

#include "screen_energy_base.h"

namespace vg {

class ScreenEnergyLight : public ScreenEnergyBase
{
public:
    ScreenEnergyLight(UniSim::Identifier name, QObject *parent);
private:
    // Parameters
    double globRad;

    // Methods
    bool useScreen();
};
} //namespace


#endif
