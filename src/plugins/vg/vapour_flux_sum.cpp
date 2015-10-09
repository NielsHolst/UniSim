/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/decode_list.h>
#include "vapour_flux_sum.h"
#include "publish.h"

using namespace UniSim;

namespace vg {

PUBLISH(VapourFluxSum)
/*! \class VapourFluxSum
 * \brief Total vapour fluxes of child models
 *
 * Inputs
 * ------
 * - _toAdd_ is a list of VapourFluxBase model to add, in addition to the child VapourFluxBase models [string vector]
 */

VapourFluxSum::VapourFluxSum(Identifier name, QObject *parent)
    : VapourFluxSumBase(name, parent)
{
    Input(QString, toAdd, "()");
}

QList<VapourFluxBase*> VapourFluxSum::fluxes() {
    return childFluxes() + referredFluxes();
}

QList<VapourFluxBase*> VapourFluxSum::childFluxes() {
    return seekChildren<VapourFluxBase*>("*");
}

QList<VapourFluxBase*> VapourFluxSum::referredFluxes() {
    QStringList modelNames = decodeList(toAdd, this);
    QList<VapourFluxBase*> models;
    for (auto modelName : modelNames) {
        auto flux = seekOne<VapourFluxBase*>(modelName);
        models << flux;
    }
    return models;
}

} //namespace

