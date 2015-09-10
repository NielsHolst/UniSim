/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREENS_H
#define VG_SCREENS_H

#include "surface_radiation_outputs.h"

namespace vg {

class Screens : public vg::SurfaceRadiationOutputs
{
public:
    Screens(UniSim::Identifier name, QObject *parent);
    void initialize();
    void reset();
    void update();
private:
    // Outputs
    double U, heatCapacity, haze, airTransmissivity, maxState;
    bool areHorizontal;

    // Data
    struct ScreenInfo {
        const bool *isHorizontal;
        const double *transmissivityLightNet,
            *absorptivityIrInnerNet, *absorptivityIrOuterNet,
            *state,
            *unhazed, *airTransmissionNet,
            *resistance, *heatCapacity;
    };
    QVector<ScreenInfo> screenInfos;

    // Methods
    QVector<ScreenInfo> collectScreenInfos(QList<Model*> screenModels);
    void updateRadiation();
};

} //namespace


#endif
