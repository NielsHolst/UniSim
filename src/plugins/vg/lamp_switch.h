/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LAMP_SWITCH_H
#define VG_LAMP_SWITCH_H

#include <usbase/model.h>

namespace vg {

class LampSwitch : public UniSim::Model
{
public:
    LampSwitch(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameters
    int day, hour, minute, spDayOn, spDayOff;
    double globRad, spHourOn, spHourOff, spRadiationOn, spRadiationOff;

    // Variables
    bool on;

};
} //namespace


#endif
