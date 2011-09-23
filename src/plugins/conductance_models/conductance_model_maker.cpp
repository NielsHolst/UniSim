/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
    "as described by @Cite{$label{Benjamin \"&\" Park (2007)}benjamin_park_2007} and references cited therein. "
    "The @F Conductance always holds three child models representing calendar, weather and plant community, "
    "respectively. The community model again holds one or two plant models.";
}

QStringList ConductanceModelMaker::authors() const {
    return QStringList()
        << "Niels" << "Mette" << "Gionata";
}

const QMap<Identifier, QString>& ConductanceModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["Community"] =
    "A @F Community object holds one or two @F Plant objects. It governs the passage of the plant(s) throught the stages of"
    "competition: @F {Unlimited}, @F {UnderCompression}, and @F {WeightProportional}.";

    desc["Plant"] =
    "A @F Plant object grows in terms of biomass, leaf area and crown zone area according to its current competition phase";

    desc["Weather"] =
    "This @F Weather object holds fixed values for global irradiation and "
    "daily average temperature.";

    return desc;
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
