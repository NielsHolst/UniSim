/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LIGHT_ON_THRESHOLD_H
#define VG_LIGHT_ON_THRESHOLD_H

#include "time_switch.h"

namespace vg {

class LowLightSwitch : public TimeSwitch
{
public:
    LowLightSwitch(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameter
    double threshold, outdoorsRadiation;

    // Variables
    bool lightOn;
};
} //namespace


#endif
