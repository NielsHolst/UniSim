/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_HUMIDITY_SETPOINTS_H
#define VG_HUMIDITY_SETPOINTS_H

#include <usbase/model.h>

namespace vg {

class HumiditySetpoints : public UniSim::Model
{
public:
    HumiditySetpoints(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    bool isDay;
    double radiation, maxRhDay, maxRhNight, minDeltaXBasis, rh, vp;
    // Variables
    double maxRh, minDeltaX;
};
} //namespace


#endif
