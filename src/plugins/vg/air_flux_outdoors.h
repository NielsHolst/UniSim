/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_AIR_FLUX_OUTDOORS_H
#define VG_AIR_FLUX_OUTDOORS_H

#include <usbase/model.h>

namespace vg {

class AirFluxOutdoors : public UniSim::Model
{
public:
    AirFluxOutdoors(UniSim::Identifier name, QObject *parent);
    void reset();
    void update();

private:
    // Inputs
    double infiltration, ventilation, volumeProportion, transmissivity;
    bool useNotTransmitted;

    // Outputs
    double value;

    // Data
    double bottomProp;
};
} //namespace


#endif
