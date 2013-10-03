/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BLACKOUT_SCREEN_CONTROLLER_H
#define VG_BLACKOUT_SCREEN_CONTROLLER_H
#include <QTime>
#include <usbase/model.h>

namespace vg {

class BlackoutScreenController : public UniSim::Model
{
public:
    BlackoutScreenController(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Parameters
    QTime fromTime, toTime, time;
    double radiationThreshold, radiation, maxSignalHighHumidity;
    bool isHumidityHigh, followEnergyScreen;

    // Variable
    double signal;

    // Links
    const double *energyScreenSignal;
};
} //namespace


#endif
