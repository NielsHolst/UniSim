/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREENS_H
#define VG_SCREENS_H

#include <QList>
#include <usbase/model.h>

namespace vg {

class Screens : public UniSim::Model
{
public:
    Screens(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Outputs
    double maxState, lightTransmission, airTransmission;
    // Data
    struct ScreenInfo {
        const double *state, *lightTransmission, *airTransmission;
    };
    QVector<ScreenInfo> screenInfos;

};
} //namespace


#endif
