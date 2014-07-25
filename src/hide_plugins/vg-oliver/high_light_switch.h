/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LIGHT_OFF_THRESHOLD_H
#define VG_LIGHT_OFF_THRESHOLD_H

#include "time_switch.h"

namespace vg {

class HighLightSwitch : public TimeSwitch
{
public:
    HighLightSwitch(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Parameter
    double threshold, outdoorsRadiation;

    // Variables
    bool lightOff;
};
} //namespace


#endif
