/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_HUMIDITY_SCREEN_BALANCE_H
#define VG_HUMIDITY_SCREEN_BALANCE_H
#include <QTime>
#include <usbase/model.h>

namespace vg {

class HumidityScreenBalance : public UniSim::Model
{
public:
    HumidityScreenBalance(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    double signalAtHighHumidity;
    bool isHumidityHigh;

    // Variable
    double signal;
};
} //namespace


#endif
