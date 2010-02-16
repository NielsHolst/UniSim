/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "simple_controller.h"
#include "standard_controller_maker.h"

namespace UniSim{

QList<Identifier> StandardControllerMaker::supportedTypes() const
{
    return QList<Identifier>()
        << Identifier("Simple");
}

UniSim::Identifier StandardControllerMaker::plugInName() const {
    return "UniSim";
}

void StandardControllerMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Controller* StandardControllerMaker::create(Identifier controllerType, Identifier objectName, QObject *parent)
{
	// Remember to add controllerType to the list above as well
    UniSim::setSimulationObjectFromDescendent(parent);
    Controller *controller = 0;
    if (controllerType.equals("Simple"))
		controller = new SimpleController(objectName, parent);
	return controller;
}

Q_EXPORT_PLUGIN2(standard_controller_maker,StandardControllerMaker)

} //namespace
