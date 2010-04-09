/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "simple_integrator.h"
#include "standard_integrator_maker.h"

namespace UniSim{

QList<Identifier> StandardIntegratorMaker::supportedTypes() const
{
    return QList<Identifier>()
        << Identifier("Simple");
}

UniSim::Identifier StandardIntegratorMaker::plugInName() const {
    return "UniSim";
}

void StandardIntegratorMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Integrator* StandardIntegratorMaker::create(Identifier integratorType, Identifier objectName, QObject *parent)
{
	// Remember to add integratorType to the list above as well
    UniSim::setSimulationObjectFromDescendent(parent);
    Integrator *integrator = 0;
    if (integratorType.equals("Simple"))
		integrator = new SimpleIntegrator(objectName, parent);
	return integrator;
}

Q_EXPORT_PLUGIN2(standard_integrator_maker,StandardIntegratorMaker)

} //namespace
