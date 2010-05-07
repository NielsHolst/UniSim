/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "beehive_model_maker.h"
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "aethina.h"
#include "fecundity.h"
#include "insect1.h"
#include "insect2.h"
#include "insect3.h"
#include "insect4.h"
#include "weather.h"

using namespace UniSim;

namespace beehive{

QList<Identifier> BeehiveModelMaker::supportedTypes() const
{
	return QList<Identifier>()
		<< Identifier("Aethina")
        << Identifier("Fecundity")
        << Identifier("Insect1")
        << Identifier("Insect2")
        << Identifier("Insect3")
        << Identifier("Insect4")
        << Identifier("Weather")
;
}

Identifier BeehiveModelMaker::plugInName() const {
	return Identifier("beehive");
}

void BeehiveModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}

Model* BeehiveModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
	if (modelType.equals("aethina"))
		model = new Aethina(objectName, parent);
    else if (modelType.equals("fecundity"))
        model = new Fecundity(objectName, parent);
    else if (modelType.equals("insect1"))
        model = new Insect1(objectName, parent);
    else if (modelType.equals("insect2"))
        model = new Insect2(objectName, parent);
    else if (modelType.equals("insect3"))
        model = new Insect3(objectName, parent);
    else if (modelType.equals("insect4"))
        model = new Insect4(objectName, parent);
    else if (modelType.equals("weather"))
        model = new Weather(objectName, parent);
    return model;
}

Q_EXPORT_PLUGIN2(beehive_model_maker, BeehiveModelMaker)

} //namespace
