/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_BASE_PERIOD_H
#define VG_BASE_PERIOD_H

#include <QTime>
#include <usbase/model.h>

namespace vg {

class TimeSwitch : public UniSim::Model
{
public:
    TimeSwitch(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();
private:
    // Parameters
    int onDay, offDay, day;
    QTime onTime, offTime, time;
protected:
    // Variables
    bool timeOn;
};
} //namespace


#endif
