/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SP_HEATING_HUMIDITY_DELTA_X_H
#define VG_SP_HEATING_HUMIDITY_DELTA_X_H

#include "sp_heating_humidity_base.h"

namespace vg {

class SpHeatingHumidityDeltaX : public SpHeatingHumidityBase
{
public:
    SpHeatingHumidityDeltaX(UniSim::Identifier name, QObject *parent);
    void update();

private:
    // Parameters
    double spDeltaX, moistureDeficit;
};
} //namespace


#endif
