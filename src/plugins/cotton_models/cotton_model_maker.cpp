/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "cotton_model_maker.h"
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "insect.h"


using namespace UniSim;

namespace cotton{

Identifier CottonModelMaker::pluginName() const {
    return Identifier("cotton");
}

QString CottonModelMaker::pluginDesc() const {
    return
    "The @F cotten plugin contains models of insect pests on feral and cultivated cotton.";
}

QStringList CottonModelMaker::authors() const {
    return QStringList()
        << "Willam Meikle, USDA-ARS, Texas, USA"
        << "Niels Holst, Aarhus University, Denmark";
}

const QMap<Identifier, QString>& CottonModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["Insect"] =
    "The @F Insect model simulated phenological development of the life stages";

    desc["Weather"] =
    "This model provides daily average temperature.";

    return desc;
}

void CottonModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}

Model* CottonModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("Insect"))
        model = new Insect(objectName, parent);
    return model;
}


Q_EXPORT_PLUGIN2(cotton_model_maker, CottonModelMaker)

} //namespace
