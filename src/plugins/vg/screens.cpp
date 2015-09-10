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
 * - _U_ is the combined U-value of all screens [W/m<SUP>2</SUP>/K]
 * - _heatCapacity_ is the heat capacity [J/K]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the screens [0;1]
 * - _transmissivityAir_ is the proportion of air transmitted through the screens [0;1]
 * - _maxState_ is the maximum screen state [0;1]
 * - _isHorizontal_ is any the screen in horizontal position ?
 */

Screens::Screens(Identifier name, QObject *parent)
    : SurfaceRadiationOutputs(name, parent)
{
    Output(bool, areHorizontal);
    Output(double, maxState);
    Output(double, airTransmissivity);
    Output(double, haze);
    Output(double, U);
    Output(double, heatCapacity);
}

void Screens::initialize() {
    auto screens = seekChildren<Model*>("*");
    screenInfos = collectScreenInfos(screens);
}

QVector<Screens::ScreenInfo> Screens::collectScreenInfos(QList<Model*> screenModels) {
    QVector<Screens::ScreenInfo> screenInfos;
    for (auto screen: screenModels) {
        screenInfos << ScreenInfo {
            screen->pullValuePtr<bool>("isHorizontal"),
            screen->pullValuePtr<double>("transmissivityLightNet"),
            screen->pullValuePtr<double>("absorptivityIrInnerNet"),
            screen->pullValuePtr<double>("absorptivityIrOuterNet"),
            screen->pullValuePtr<double>("state"),
            screen->pullValuePtr<double>("unhazed"),
            screen->pullValuePtr<double>("transmissivityAirNet"),
            screen->pullValuePtr<double>("resistance"),
            screen->pullValuePtr<double>("heatCapacity")

        };
    }
    return screenInfos;
}

void Screens::reset() {
    areHorizontal = false;
    for (ScreenInfo info: screenInfos)
        areHorizontal = areHorizontal || (*info.isHorizontal);
    resetRadiationOutputs();
    maxState = haze = 0;
    airTransmissivity = 1;
    U = numeric_limits<double>::infinity();
    heatCapacity = 0.;
}

void Screens::update() {
    maxState = 0;
    airTransmissivity = 1;
    heatCapacity = 0;
    double resistance{0}, unhazed{1};
    for (ScreenInfo info: screenInfos) {
        maxState = max(maxState, *info.state);
        airTransmissivity *= *info.airTransmissionNet;
        unhazed *= *info.unhazed;
        resistance += *info.resistance;
        heatCapacity += *info.heatCapacity;
    }
    haze = 1. - unhazed;
    U = 1./resistance;

    SurfaceRadiation rad;
    for (ScreenInfo si: screenInfos) {
        rad *= SurfaceRadiation().asScreen(*si.transmissivityLightNet, *si.absorptivityIrOuterNet, *si.absorptivityIrInnerNet);
    }
    set(rad);
}

} //namespace

