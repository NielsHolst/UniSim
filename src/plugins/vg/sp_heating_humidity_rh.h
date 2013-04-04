/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SP_HEATING_HUMIDITY_RH_H
#define VG_SP_HEATING_HUMIDITY_RH_H

#include "sp_heating_humidity_base.h"

namespace vg {

class SpHeatingHumidityRh : public SpHeatingHumidityBase
{
public:
    SpHeatingHumidityRh(UniSim::Identifier name, QObject *parent);
    void update();

private:
    // Parameters
    double spRh, rh;
};
} //namespace


#endif
