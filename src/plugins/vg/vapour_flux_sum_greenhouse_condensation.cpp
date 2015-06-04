/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "vapour_flux_sum_greenhouse_condensation.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(VapourFluxSumGreenhouseCondensation)
/*! \class VapourFluxSumGreenhouseCondensation
 * \brief Total condensation on greenhouse cover and screens
 */

VapourFluxSumGreenhouseCondensation::VapourFluxSumGreenhouseCondensation(Identifier name, QObject *parent)
    : VapourFluxSumBase(name, parent)
{
}

QList<VapourFluxBase*> VapourFluxSumGreenhouseCondensation::fluxes() {
    auto condensations = seekMany<VapourFluxBase*>("greenhouseShelter/*/cover/condensation");
    if (condensations.size() != 6) {
        QString msg{"Greenhouse must have 6 shelters, each with a cover with a condensation model inside (found %1)"};
        throw Exception(msg.arg(condensations.size()));
    }
    return condensations;
}

} //namespace

