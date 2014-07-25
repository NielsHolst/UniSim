/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_CO2_SETPOINTS_H
#define VG_CO2_SETPOINTS_H

#include <QTime>
#include <usbase/model.h>

namespace vg {

class Co2Setpoints : public UniSim::Model
{
public:
    Co2Setpoints(UniSim::Identifier name, QObject *parent);
//    void amend();
//    void reset();
//    void update();

private:
    // Variables
    double signal;

//    // Links
//    QList<const bool *> timeSwitchesOn, highLightSwitchesOff, lowLightSwitchesOn;

//    // Methods
//    bool onPeriod();
};
} //namespace


#endif
