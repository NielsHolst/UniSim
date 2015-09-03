/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_COVER_H
#define VG_COVER_H

#include <usbase/model.h>
#include "surface_radiation_outputs.h"

namespace UniSim {
    class DataGrid;
}

namespace vg {

class Cover : public vg::SurfaceRadiationOutputs
{
public:
    Cover(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    QString directTransmissionFile;
    double latitude, azimuth,
        U4, windspeed, emissivity, absorptivity, transmissivity, heatCapacity, haze;
    bool antiReflection;

    // Outputs
    double U;

    // Data
    UniSim::DataGrid *dirTransTable;
};
} //namespace


#endif
