/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SHELTERS_H
#define VG_SHELTERS_H

#include <usbase/model.h>

namespace vg {

class Shelters : public UniSim::Model
{
public:
    Shelters(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();

private:
    // Outputs
    double airTransmissivity, U, area, maxScreenState;
    // Data
    struct ShelterInfo {
        const double *pAirTransmissivity, *pU, *pArea, *pMaxState;
    };

    QVector<ShelterInfo> infos;
};
} //namespace


#endif
