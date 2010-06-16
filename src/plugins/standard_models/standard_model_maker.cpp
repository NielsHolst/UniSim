/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "anonymous_model.h"
#include "calendar.h"
#include "day_degrees.h"
#include "days.h"
#include "lactin_time.h"
#include "photo_thermal_time.h"
#include "stage.h"
#include "standard_model_maker.h"

namespace UniSim{

UniSim::Identifier StandardModelMaker::pluginName() const {
    return "UniSim";
}

QString StandardModelMaker::pluginDesc() const {
    return
    "The @F UniSim plugin contains a collection of models of general utility.";
}

QStringList StandardModelMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark";
}

const QMap<Identifier, QString>& StandardModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["Anonymous"] =
    "@I pending";

    desc["Calendar"] =
    "@I pending";

    desc["DayDegrees"] =
    "@I pending";

    desc["Days"] =
    "@I pending";

    desc["LactinTime"] =
    "@I pending";

    desc["PhotoThermalTime"] =
    "@I pending";

    desc["Stage"] =
    "@I pending";

    return desc;
}

void StandardModelMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Model* StandardModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	// Remember to add modelType to the list above as well
    UniSim::setSimulationObjectFromDescendent(parent);
    Model *model=0;
    if (modelType.equals("Anonymous"))
		model = new AnonymousModel(objectName, parent);
    else if (modelType.equals("Calendar"))
        model = new Calendar(objectName, parent);
    else if (modelType.equals("DayDegrees"))
        model = new DayDegrees(objectName, parent);
    else if (modelType.equals("Days"))
        model = new Days(objectName, parent);
    else if (modelType.equals("LactinTime"))
        model = new LactinTime(objectName, parent);
    else if (modelType.equals("PhotoThermalTime"))
        model = new PhotoThermalTime(objectName, parent);
    else if (modelType.equals("Stage"))
        model = new Stage(objectName, parent);
    return model;
}

Q_EXPORT_PLUGIN2(standard_model_maker,StandardModelMaker)

} //namespace
