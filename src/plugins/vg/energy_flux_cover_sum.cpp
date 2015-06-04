/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "energy_flux_cover_sum.h"
#include "general.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(EnergyFluxCoverSum)

/*! \class EnergyFluxCoverSum
 * \brief Flux of energy lost through all covers total
 */

EnergyFluxCoverSum::EnergyFluxCoverSum(Identifier name, QObject *parent)
    : EnergyFluxBase(name, parent)
{
    InputRef(double, greenhouseArea, "construction/geometry[groundArea]");
}

void EnergyFluxCoverSum::reset() {
    covers.clear();
    auto coverModels = seekMany<Model*>("greenhouseShelter/*/cover");
    for (auto cover : coverModels) {
        CoverInfo ci;
        ci.area = cover->pullValuePtr<double>("area");
        ci.flux = cover->seekOneChild<Model*>("energyFlux")->pullValuePtr<double>("value");
        covers << ci;
    }
}

void EnergyFluxCoverSum::update() {
    value = 0;
    for (auto cover : covers) {
        value += cover.totalFlux();
    }
    value /= greenhouseArea;
}

} //namespace

