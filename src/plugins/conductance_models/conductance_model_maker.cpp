/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "conductance_model_maker.h"
#include "community.h"
#include "plant.h"
#include "weather.h"

using namespace UniSim;

namespace conductance{

Identifier ConductanceModelMaker::pluginName() const {
    return Identifier("conductance");
}

QString ConductanceModelMaker::pluginDesc() const {
    return
    "The @F conductance plugin is an implementation of the Conductance model for plant growth and competition, "
    "as described by @Cite{$benjamin_park_2007} and references cited therein."
    "@PP A @Conductance model must hold as children, one of two objects of @F Plant class.";
}

QStringList ConductanceModelMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark"
        << "Mette Søndergård, Aarhus University, Denmark"
        << "Gionata Bocci, Scuola Superiore Sant'Anna of Pisa, Italy";
}

const QMap<Identifier, QString>& ConductanceModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["Community"] =
    "@I pending";

    desc["Plant"] =
    "@I pending";

    desc["Weather"] =
    "@I pending";

    return desc;
}

void ConductanceModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}

Model* ConductanceModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("community"))
        model = new Community(objectName, parent);
    else if (modelType.equals("plant"))
		model = new Plant(objectName, parent);
	else if (modelType.equals("weather"))
		model = new Weather(objectName, parent);
	return model;
}

Q_EXPORT_PLUGIN2(conductance_model_maker, ConductanceModelMaker)

} //namespace
