/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "time_limited.h"
#include "time_step_limited.h"
#include "unisim_integrator_maker.h"

namespace UniSim{

UniSim::Identifier UniSimIntegratorMaker::pluginName() const {
    return "UniSim";
}

QString UniSimIntegratorMaker::pluginDesc() const {
    return
    "The @F UniSim plugin contains a collection of integrators of general utility.";
}

QStringList UniSimIntegratorMaker::authors() const {
    return QStringList()
        << "Niels";
}

const QMap<Identifier, QString>& UniSimIntegratorMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["TimeLimited"] =
    "@I pending";

    desc["TimeStepLimited"] =
    "@I pending";

    return desc;
}

void UniSimIntegratorMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Integrator* UniSimIntegratorMaker::create(Identifier integratorType, Identifier objectName, QObject *parent)
{
	// Remember to add integratorType to the list above as well
    UniSim::setSimulationObjectFromDescendent(parent);
    Integrator *integrator = 0;
    if (integratorType.equals("TimeLimited"))
        integrator = new TimeLimited(objectName, parent);
    else if (integratorType.equals("TimeStepLimited"))
        integrator = new TimeStepLimited(objectName, parent);
    return integrator;
}

Q_EXPORT_PLUGIN2(unisim_integrator_maker, UniSimIntegratorMaker)

} //namespace
