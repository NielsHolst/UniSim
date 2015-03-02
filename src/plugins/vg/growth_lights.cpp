/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "growth_lights.h"
#include "publish.h"
using namespace UniSim;

namespace vg {

PUBLISH(GrowthLights)

/*! \class GrowthLights
 * \brief A collection of GrowthLight models
 *
 * Optional dependencies
 * ------------
 * - some (0..n) GrowthLight child models
 */

GrowthLights::GrowthLights(Identifier name, QObject *parent)
    : BaseGrowthLight(name, parent)
{
}

void GrowthLights::initialize() {
    auto lights = seekChildren<BaseGrowthLight*>("*");
    for (auto light : lights) {
        pHeatEmission << light->pullValuePtr<double>("heatEmission");
        pLongWaveEmission << light->pullValuePtr<double>("longWaveEmission");
        pShortWaveEmission << light->pullValuePtr<double>("shortWaveEmission");
        pParEmission << light->pullValuePtr<double>("parEmission");
        pEnergyUse << light->pullValuePtr<double>("energyUse");
        pCurrentlyOn << light->pullValuePtr<bool>("currentlyOn");
    }
}

void GrowthLights::reset() {
    noLight();
}

namespace {
    double sum(QVector<const double*> &values) {
        double total{0};
        for (auto value : values)
            total += *value;
        return total;
    }

    double unite(QVector<const bool*> &values) {
        bool total{false};
        for (auto value : values)
            total = total || *value;
        return total;
    }
}
void GrowthLights::update() {
    heatEmission = sum(pHeatEmission);
    longWaveEmission = sum(pLongWaveEmission);
    shortWaveEmission = sum(pShortWaveEmission);
    parEmission = sum(pParEmission);
    energyUse = sum(pEnergyUse);
    currentlyOn = unite(pCurrentlyOn);
}

} //namespace

