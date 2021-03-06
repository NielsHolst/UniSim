/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <stdlib.h>
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/test_num.h>
#include "general.h"
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
 * - _areHorizontal_ tells if these screens are horizontal [true/false]
 * - _maxState_ is the maximum screen state [0;1]
 * - _airTransmissivity_ is the proportion of air transmitted through the screens [0;1]
 * - _haze_ is the proportion of direct light becoming dispersed on passage through the screens [0;1]
 * - _U_ is the combined U-value of all screens [W/m<SUP>2</SUP>/K]
 * - _heatCapacity_ is the heat capacity [J/K]
 * - _effectiveArea_ is the draw area of the screens
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
    Output(double, effectiveArea);
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
            screen->pullValuePtr<double>("absorptivityLwInnerNet"),
            screen->pullValuePtr<double>("absorptivityLwOuterNet"),
            screen->pullValuePtr<double>("state"),
            screen->pullValuePtr<double>("unhazed"),
            screen->pullValuePtr<double>("transmissivityAirNet"),
            screen->pullValuePtr<double>("resistance"),
            screen->pullValuePtr<double>("heatCapacity"),
            screen->pullValuePtr<double>("effectiveArea")
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
    U = infinity();
    heatCapacity = effectiveArea = 0.;
}

void Screens::update() {
    maxState = 0;
    airTransmissivity = 1;
    heatCapacity =
    effectiveArea  = 0;
    double resistance{0}, unhazed{1};
    for (ScreenInfo info: screenInfos) {
        maxState = max(maxState, *info.state);
        airTransmissivity *= *info.airTransmissionNet;
        unhazed *= *info.unhazed;
        resistance += *info.resistance;
        heatCapacity += *info.heatCapacity;
        effectiveArea += *info.effectiveArea;
    }
    TestNum::snapToZero(maxState);
    haze = 1. - unhazed;
    U = 1./resistance;

    SurfaceRadiation rad;
    for (ScreenInfo si: screenInfos) {
        SurfaceRadiation &screenRad( SurfaceRadiation().asScreen(*si.transmissivityLightNet, *si.absorptivityLwOuterNet, *si.absorptivityLwInnerNet) );
        rad *= screenRad;
    }
    set(rad);
}

} //namespace

