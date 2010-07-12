/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "grainstore_model_maker.h"
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "larva_time.h"
#include "maize.h"
#include "prostephanus.h"
#include "pt_development.h"
#include "pt_emigration.h"
#include "pt_immigration.h"
#include "pt_reproduction.h"
#include "weather.h"

using namespace UniSim;

namespace grainstore{

Identifier GrainstoreModelMaker::pluginName() const {
    return Identifier("grainstore");
}

QString GrainstoreModelMaker::pluginDesc() const {
    return
    "The @F grainstore plugin contains models that simulate the population dynamics"
    "of the major insect pests (@I {Prostephanus truncatus} and @I {Sitophilus zeamais})"
    "and the biocontrol agent @I {Teretrius nigrescens} in African maize stores, "
    "as described by @Cite{$meikle_et_al_1998, $meikle_et_al_1999, $holst_meikle_2003}."
    "@PP The @F grainstore plugin is still under development and not yet functional.";
}

QStringList GrainstoreModelMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark";
}


const QMap<Identifier, QString>& GrainstoreModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["LarvaTime"] =
    "@I pending";

    desc["Maize"] =
    "@I pending";

    desc["Prostephanus"] =
    "@I pending";

    desc["PtDevelopment"] =
    "@I pending";

    desc["PtEmigration"] =
    "@I pending";

    desc["PtImmigration"] =
    "@I pending";

    desc["PtReproduction"] =
    "@I pending";

    desc["Weather"] =
    "@I pending";

    return desc;
}

void GrainstoreModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}

Model* GrainstoreModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("LarvaTime"))
        model = new LarvaTime(objectName, parent);
    else if (modelType.equals("Maize"))
		model = new Maize(objectName, parent);
    else if (modelType.equals("Prostephanus"))
		model = new Prostephanus(objectName, parent);
    else if (modelType.equals("PtDevelopment"))
		model = new PtDevelopment(objectName, parent);
    else if (modelType.equals("PtEmigration"))
		model = new PtEmigration(objectName, parent);
    else if (modelType.equals("PtImmigration"))
        model = new PtImmigration(objectName, parent);
    else if (modelType.equals("PtReproduction"))
		model = new PtReproduction(objectName, parent);
    else if (modelType.equals("Weather"))
        model = new Weather(objectName, parent);
return model;
}

Q_EXPORT_PLUGIN2(grainstore_model_maker, GrainstoreModelMaker)

} //namespace
