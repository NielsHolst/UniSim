/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/decode_list.h>
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
    Input(QString, toAdd, "()");
    InputRef(double, greenhouseArea, "geometry[groundArea]");
}

void EnergyFluxCoverSum::reset() {
    QStringList modelNames = decodeList(toAdd, this);
    coverInfos.clear();
    for (auto modelName : modelNames) {
        Model *energyFlux = seekOne<Model*>(modelName),
              *cover = energyFlux->seekParent<Model*>("*"),
              *shelter = cover->seekParent<Model*>("*");
        CoverInfo ci {
            energyFlux->pullValuePtr<double>("value"),
            shelter->pullValuePtr<double>("area")
        };
        coverInfos << ci;
    }
}

void EnergyFluxCoverSum::update() {
    value = 0;
    for (auto ci : coverInfos) {
        value += ci.totalFlux();
    }
    value /= greenhouseArea;
}

} //namespace

