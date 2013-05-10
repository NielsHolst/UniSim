/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SP_HUMIDITY_REGULAR_H
#define VG_SP_HUMIDITY_REGULAR_H

#include "sp_humidity_base.h"

namespace vg {

class SpHumidityRegular : public SpHumidityBase
{
public:
    SpHumidityRegular(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    double globRad, spDaylight, spHumidityDay, spHumidityNight, spDeltaXBasis;
};
} //namespace


#endif