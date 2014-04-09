/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_LIGHT_CONTROLLER_H
#define VG_LIGHT_CONTROLLER_H

#include <QTime>
#include <usbase/model.h>

namespace vg {

class LightController : public UniSim::Model
{
public:
    LightController(UniSim::Identifier name, QObject *parent);
    void amend();
    void reset();
    void update();

private:
    // Variables
    double signal;

    // Links
    QList<const bool *> timeSwitchesOn, highLightSwitchesOff, lowLightSwitchesOn;

    // Methods
    bool onPeriod();
};
} //namespace


#endif
