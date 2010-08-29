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
#include "random_lognormal.h"
#include "random_normal.h"
#include "random_poisson.h"
#include "random_uniform.h"
#include "stage.h"
#include "unisim_model_maker.h"

namespace UniSim{

UniSim::Identifier UniSimModelMaker::pluginName() const {
    return "UniSim";
}

QString UniSimModelMaker::pluginDesc() const {
    return
    "The @F UniSim plugin contains a collection of models of general utility.";
}

QStringList UniSimModelMaker::authors() const {
    return QStringList()
        << "Niels Holst, Aarhus University, Denmark";
}

const QMap<Identifier, QString>& UniSimModelMaker::supportedClasses() {
    if (!desc.isEmpty())
        return desc;

    desc["Anonymous"] =
    "An @F Anonymous model simple acts as a container of other models";

    desc["Calendar"] =
    "The @F Calendar model keeps track of the date. Since @F latitude is one of its parameters, "
    "it also knows of the current day length. Solar elevation is calculated every time "
    "the hour of the day is set by a tick of the global clock object";

    desc["DayDegrees"] =
    "This is a standard day-degree model. It obtains the daily average temperature from "
    "the weather object, which must exist as @F Model named @F {weather} having a pull "
    "variable named @F {Tavg}. The daily increment in day-degrees increases linerly above "
    "@F T0 until @F {Topt}. Above @F Tops it decreases linearly until @F {Tmax}.";

    desc["Days"] =
    "This model has the same pull variables as the @F DayDegrees model but it works in "
    "simple chronological time counting every day as just that, one day.";

    desc["LactinTime"] =
    "This is the physiological time scale of Lactin et al. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.";

    desc["PhotoThermalTime"] =
    "Photothermal time accounts for temperature and day length at the same time. "
    "In this implementation the daily increment as calculated by the @F DayDegrees model "
    "is multiplied by day length in hours (acquired from the @F weather object) "
    "divided by 24 hours.";

    desc["RandomLognormal"] =
    "The model maintains a random variable with lognormal distrubution.";

    desc["RandomNormal"] =
    "The model maintains a random variable with normal distrubution.";

    desc["RandomPoisson"] =
    "The model maintains a random variable with Poisson distrubution.";

    desc["RandomUniform"] =
    "The model maintains a random variable with uniform (even) distrubution.";

    desc["Stage"] =
    "The @F Stage model has an a distributed delay routine @Cite{$manetsch, $vansickle} inside. "
    "The implementation follows the original FORTRAN code @Cite{$abkin}. The two parameters @F "
    "duration and @F k determine the average and variance of stage duration with variance equa to"
    "@Math {@F duration sup 2 slash @F k sup 2}. The time scale used by the @F Stage model is "
    "determined by an obligatory child model named @F {time}.";

    return desc;
}

void UniSimModelMaker::useObjectPool(ObjectPool *pool) const {
    objectPool()->deferTo(pool);
}

Model* UniSimModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
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
    else if (modelType.equals("RandomLognormal"))
        model = new RandomLognormal(objectName, parent);
    else if (modelType.equals("RandomNormal"))
        model = new RandomNormal(objectName, parent);
    else if (modelType.equals("RandomPoisson"))
        model = new RandomPoisson(objectName, parent);
    else if (modelType.equals("RandomUniform"))
        model = new RandomUniform(objectName, parent);
    else if (modelType.equals("Stage"))
        model = new Stage(objectName, parent);
    return model;
}

Q_EXPORT_PLUGIN2(unisim_model_maker,UniSimModelMaker)

} //namespace
