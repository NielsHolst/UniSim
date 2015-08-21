/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <QMessageBox>
#include <usbase/exception.h>
#include "publish.h"
#include "screens.h"
#include "surface_radiation.h"

using std::max;
using namespace UniSim;

namespace vg {

PUBLISH(Screens)


/*! \class Screens
 * \brief A collection of Screen objects
 *
 * Outputs
 * ------
 * - _maxState_ is the maximum value of the screen states [0;1]
 */

Screens::Screens(Identifier name, QObject *parent)
    : Model(name, parent)
{
    Input(QString, additionalScreens, QString());

    Output(double, lightTransmissivity);
    Output(double, irTransmissivity);
    Output(double, incomingLightAbsorptivity);
    Output(double, incomingLightReflectivity);
    Output(double, incomingIrAbsorptivity);
    Output(double, incomingIrReflectivity);
    Output(double, outgoingLightAbsorptivity);
    Output(double, outgoingLightReflectivity);
    Output(double, outgoingIrAbsorptivity);
    Output(double, outgoingIrReflectivity);

    Output(double, maxState);
    Output(double, airTransmissivity);
    Output(double, haze);
    Output(double, U);
}

void Screens::initialize() {
    auto screens = seekChildren<Model*>("*");
    screenInfos = collectScreenInfos(screens);
    screenInfosPlus.clear();
    if (!additionalScreens.isEmpty()) {
        auto additionalScreenModels = seekOne<Model*>(additionalScreens);
        auto screensPlus = additionalScreenModels->seekChildren<Model*>("*");
        screenInfosPlus = collectScreenInfos(screensPlus);
    }
}

QVector<Screens::ScreenInfo> Screens::collectScreenInfos(QList<Model*> screenModels) {
    QVector<Screens::ScreenInfo> screenInfos;
    for (auto screen: screenModels) {
        screenInfos << ScreenInfo {
            screen->pullValuePtr<double>("transmissivityLightNet"),
            screen->pullValuePtr<double>("absorptivityIrInnerNet"),
            screen->pullValuePtr<double>("absorptivityIrOuterNet"),
            screen->pullValuePtr<double>("state"),
            screen->pullValuePtr<double>("unhazed"),
            screen->pullValuePtr<double>("transmissivityAirNet"),
            screen->pullValuePtr<double>("resistance")
        };
    }
    return screenInfos;
}

void Screens::reset() {
    maxState = 0;
    lightTransmissivity = airTransmissivity = 1;
    U = numeric_limits<double>::infinity();
}

void Screens::update() {
    maxState = 0;
    airTransmissivity = 1;
    double resistance{0}, unhazed{1};
    for (ScreenInfo info: screenInfos) {
        maxState = max(maxState, *info.state);
        airTransmissivity *= *info.airTransmissionNet;
        unhazed *= *info.unhazed;
        resistance += *info.resistance;
    }
    haze = 1. - unhazed;
    U = 1./resistance;
    updateRadiation();
}


void Screens::updateRadiation() {
    SurfaceRadiation rad;
    for (ScreenInfo si: screenInfos) {
        rad *= SurfaceRadiation(*si.transmissivityLightNet, *si.absorptivityIrOuterNet, *si.absorptivityIrInnerNet);
    }
    lightTransmissivity = rad.light.tra;
    irTransmissivity = rad.ir.tra;
    incomingLightAbsorptivity = rad.light.outer.abs;
    incomingLightReflectivity = rad.light.outer.ref;
    incomingIrAbsorptivity = rad.ir.outer.abs;
    incomingIrReflectivity = rad.ir.outer.ref;
    outgoingLightAbsorptivity = rad.light.inner.abs;
    outgoingLightReflectivity = rad.light.inner.ref;
    outgoingIrAbsorptivity = rad.ir.inner.abs;
    outgoingIrReflectivity = rad.ir.inner.ref;
}

} //namespace

