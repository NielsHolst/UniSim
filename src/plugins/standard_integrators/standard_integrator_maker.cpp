/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "time_limited.h"
#include "time_step_limited.h"
#include "standard_integrator_maker.h"

namespace UniSim{

UniSim::Identifier StandardIntegratorMaker::pluginName() const {
    return "UniSim";
}

QString StandardIntegratorMaker::pluginDesc() const {
    return
    "The @F UniSim plugin contains a collection of integrators of general utility.";
}

QStringList StandardIntegratorMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark";
}

const QMap<Identifier, QString>& StandardIntegratorMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["TimeLimited"] =
    "@I pending";

    desc["TimeStepLimited"] =
    "@I pending";

    return desc;
}

void StandardIntegratorMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Integrator* StandardIntegratorMaker::create(Identifier integratorType, Identifier objectName, QObject *parent)
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

Q_EXPORT_PLUGIN2(standard_integrator_maker,StandardIntegratorMaker)

} //namespace
