/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef VG_SCREENS_H
#define VG_SCREENS_H

#include <stdlib.h>
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
    // Inputs
    QString additionalScreens;

    // Outputs
    double lightTransmissivity, irTransmissivity,
           incomingLightAbsorptivity, incomingLightReflectivity,
           incomingIrAbsorptivity, incomingIrReflectivity,
           outgoingLightAbsorptivity, outgoingLightReflectivity,
           outgoingIrAbsorptivity, outgoingIrReflectivity,
           maxState, airTransmissivity, haze, U;
    // Data
    struct ScreenInfo {
        const double *transmissivityLightNet,
            *absorptivityIrInnerNet, *absorptivityIrOuterNet,
            *state,
            *unhazed, *airTransmissionNet,
            *resistance;
    };
    QVector<ScreenInfo> screenInfos, screenInfosPlus;

    // Methods
    QVector<ScreenInfo> collectScreenInfos(QList<Model*> screenModels);
    void updateRadiation();
};

} //namespace


#endif
