/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "beehive_model_maker.h"
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "egg_survival.h"
#include "fecundity.h"
#include "insect1.h"
#include "insect2.h"
#include "insect4.h"
#include "insect6.h"
#include "weather.h"

using namespace UniSim;

namespace beehive{

Identifier BeehiveModelMaker::pluginName() const {
    return Identifier("beehive");
}

QString BeehiveModelMaker::pluginDesc() const {
    return
    "The @F beehive plugin contains a series of models, "
    "demonstrating the steps in the construction of an insect model.";
}

QStringList BeehiveModelMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark"
        << "Willam Meikle, USDA-ARS, Texas, USA";
}

const QMap<Identifier, QString>& BeehiveModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["EggSurvival"] =
    "The model is a parabola: @Math{f(x) =ax sup 2 + bx + c} meant to describe "
    "the temperature-dependent survival of eggs. "
    "@F EggSurvival looks for a model called @F weather from which is pulls "
    "the @F TAvg variable, which is used as @I x in the parabolic equation. "
    "For temperatures less than 21 @Char{ring}C, the curve is extrapolated "
    " conservatively as @I{f}(21). ";

    desc["Fecundity"] =
    "The model is a parabola: @Math{f(x) =scale(x - \"root1\")(x - \"root2\")} meant to "
    "describe the age-specific fecundity of adults. "
    "@F Fecundity looks up an ascendant model called @F adult which should be of "
    "@F UniSim::Stage type.";

    desc["Insect1"] =
    "In this first step of building an insect model, we only have the egg stage "
    "developing on a chronological scale (days). The three subsequent life stages "
    "are present but are not connected to the preceeding stage. The life stages "
    "must be present as children of @F Insect1 and called @F {egg}, @F {larva}, "
    "@F pupa and @F {adult}.";

    desc["Insect2"] =
    "In this step all four life stages are connected, outflow from one stage "
    "passed on as inflow to the next stage.";

    desc["Insect4"] =
    "This model looks for a sibling called @F fecundity from which it pulls the "
    "@F eggsLaid variable. This is used to provide an inflow to the @F egg stage.";

    desc["Insect6"] =
    "This model looks for a sibling called @F fecundity from which it pulls the "
    "@F eggsLaid variable. This is used to provide an inflow to the @F egg stage.";

    desc["Weather"] =
    "This model provides daily average temperature.";

    return desc;
}

void BeehiveModelMaker::useObjectPool(ObjectPool *pool) const {
	objectPool()->deferTo(pool);
}

Model* BeehiveModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("EggSurvival"))
        model = new EggSurvival(objectName, parent);
    else if (modelType.equals("Fecundity"))
        model = new Fecundity(objectName, parent);
    else if (modelType.equals("Insect1"))
        model = new Insect1(objectName, parent);
    else if (modelType.equals("Insect2"))
        model = new Insect2(objectName, parent);
    else if (modelType.equals("Insect4"))
        model = new Insect4(objectName, parent);
    else if (modelType.equals("Insect6"))
        model = new Insect6(objectName, parent);
    else if (modelType.equals("Weather"))
        model = new Weather(objectName, parent);
    return model;
}


Q_EXPORT_PLUGIN2(beehive_model_maker, BeehiveModelMaker)

} //namespace
