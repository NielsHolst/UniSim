/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include "exponential.h"
#include "fixed.h"
#include "hydro_thermal_time.h"
#include "insect.h"
#include "lactin_time.h"
#include "photo_thermal_time.h"
#include "random_lognormal.h"
#include "random_normal.h"
#include "random_poisson.h"
#include "random_uniform.h"
#include "records.h"
#include "run_iterator_fixed.h"
#include "scheduled.h"
#include "stage.h"
#include "trigger_by_date.h"
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
        << "Niels";
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

    desc["Exponential"] =
    "Simple exponential growth model, @Math{y = exp(rt)}, "
    "where @I t is taken from the nearest model called @F {time}.";

    desc["Fixed"] =
    "@I Pending...";

    desc["HydroThermalTime"] =
    "Hydrothermal time accounts for temperature and soil water potential at the same time. "
    "In this implementation the daily increment, as calculated by the @F DayDegrees model, "
    "is set to zero if soil water potential is less than the threshold, otherwise the "
    "daily increment is unaltered. A @F Model named @F {weather} with a pull "
    "variable named @F SWP must exist to supply @F HydroThermalTime with soil water potential.";

    desc["Insect"] =
    "A stage-structured insect model. Under development";

    desc["LactinTime"] =
    "This is the physiological time scale of Lactin et al. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.";

    desc["PhotoThermalTime"] =
    "Photothermal time accounts for temperature and day length at the same time. "
    "In this implementation the daily increment, as calculated by the @F DayDegrees model, "
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

    desc["RunIteratorFixed"] =
    "The model increments its @F iteration counter for every $F {update}. "
    "Its @F value remains true as long as @Math{@F{iteration <= numIterations}}.";

    desc["Records"] =
    "A @F Records model reads input from a text file formatted in columns with labels in the first line. "
    "Columns can be separated by spaces or tab characters. Labels may not contain spaces. "
    "A pull variable will be created for every column in the input file, named after the column label. "
    "There can be more than one @F Records model present in a simulation. It is the @F calendar model "
    "that determines the current date and time of day, not the @F Records model(s)."
    "@PP Columns named @F date and @F time have special meanings, as they are used to synchronize the "
    "readings from the input file with the @F calendar model. You must have either a @F date or a @F time "
    "column, or both. The values from the first and last line of the input file will be extrapolated into the time "
    "period before and after, respectively, as needed. "
    "@PP The pull variables for date and time are mostly used for test purposes. If you "
    "need to know about the current date and time of day, you should pull this information from "
    "the @F calender model. See UniSim::Calendar @CrossLink {page @PageOf UniSim::Calendar}.";

    desc["Scheduled"] =
    "A @F Scheduled model produces canned data which are derived from a list of time value pairs, e.g., "
    "@F {((10.2 14.6) (45.1 32.3) (57.1 24.43))}. Values are interpolated from the current time "
    "which is taken from the nearest model called @F {time}. "
    "At times outside the range covered by the list (outside [10.2;57.1], in this case) values are extrapolated "
    "(to 14.6 or 24.43), in this case).";

    desc["Stage"] =
    "The @F Stage model holds a distributed delay routine (@Cite{$label{Manetsch 1976}manetsch, $label{Vansickle 1977}vansickle}) inside. "
    "The implementation follows the original FORTRAN code of @Cite{$label{Abkin \"&\" Wolf (1976)}abkin}. The two parameters @F "
    "duration and @F k determine the average and variance of stage duration with variance equal to "
    "@Math{@F duration sup 2 slash @F k sup 2}. The time scale used by the @F Stage model is "
    "determined by an obligatory child model named @F {time}.";

    desc["TriggerByDate"] =
    "The  @F TriggerByDate model can be used by other models to trigger changes in their behaviour.";

    return desc;
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
    else if (modelType.equals("Exponential"))
        model = new Exponential(objectName, parent);
    else if (modelType.equals("Fixed"))
        model = new Fixed(objectName, parent);
    else if (modelType.equals("HydroThermalTime"))
        model = new HydroThermalTime(objectName, parent);
    else if (modelType.equals("Insect"))
        model = new Insect(objectName, parent);
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
    else if (modelType.equals("Records"))
        model = new Records(objectName, parent);
    else if (modelType.equals("RunIteratorFixed"))
        model = new RunIteratorFixed(objectName, parent);
    else if (modelType.equals("Scheduled"))
        model = new Scheduled(objectName, parent);
    else if (modelType.equals("Stage"))
        model = new Stage(objectName, parent);
    else if (modelType.equals("TriggerByDate"))
        model = new TriggerByDate(objectName, parent);
    return model;
}

Q_EXPORT_PLUGIN2(unisim_model_maker,UniSimModelMaker)

} //namespace
