/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include "functional_response_g_b.h"
#include "hydro_thermal_time.h"
#include "infection.h"
#include "inachis.h"
#include "insect_life_cycle.h"
#include "lactin_time.h"
#include "output_crosstab.h"
#include "output_map.h"
#include "output_plot.h"
#include "output_table.h"
#include "photo_thermal_time.h"
#include "predation.h"
#include "random_lognormal.h"
#include "random_normal.h"
#include "random_poisson.h"
#include "random_uniform.h"
#include "range.h"
#include "records.h"
#include "run_iterator_fixed.h"
#include "scheduled.h"
#include "sensitivity_analysis.h"
#include "stage.h"
#include "stage_demand.h"
#include "stage_and_phase.h"
#include "time_limited.h"
#include "steps.h"
#include "sum.h"
#include "trigger_by_date.h"
#include "unisim_factory.h"
#include "weather.h"

namespace UniSim{

void UniSim::UniSimFactory::defineProducts() {
    addProduct<AnonymousModel>("Anonymous", this,
    "An @F Anonymous model simply acts as a container of other models");

    addProduct<Calendar>("Calendar", this,
    "The @F Calendar model keeps track of the date. Since @F latitude is one of its parameters, "
    "it also knows of the current day length. Solar elevation is calculated every time "
    "the hour of the day is set by a tick of the global clock object. For example,"
    "@IndentedDisplay @CP tabout{0.3i}{"
    "#include <usbase/clock.h>\n"
    "double hour = 12.5; // Half-past noon\n"
    "clock()->doTick(hour);\n}");

    addProduct<OutputCrosstab>("CrossTab", this, "Output class. Description pending");

    addProduct<DayDegrees>("DayDegrees", this,
    "This is a standard day-degree model. It obtains the daily average temperature from "
    "the weather object, which must exist as @F Model named @F {weather} having a pull "
    "variable named @F {Tavg}. The daily increment in day-degrees increases linerly above "
    "@F T0 until @F {Topt}. Above @F Tops it decreases linearly until @F {Tmax}.\n"
    "@PP\n The @F DayDegree model is derived from the @F PhysiologicalTime base class, "
    "like the alternatives: @F {Days}, @F {HydroThermalTime}, @F {LactinTime} and "
    " @F {PhotoThermalTime}. They all start updating right from the beginning, or when triggered "
    "by a child model, for example a @F TriggerByDate model.");

    addProduct<Days>("Days", this,
    "This model has the same pull variables as the @F DayDegrees model but it works in "
    "simple chronological time counting every day as just that, one day.");

    addProduct<Exponential>("Exponential", this,
    "Simple exponential growth model, @Math{y = exp(rt)}, "
    "where @I t is taken from the nearest model called @F {time}.");

    addProduct<Fixed>("Fixed", this,
    "Often used for test purposes, or an initial version of a model, a @F Fixed model "
    "sets up fixed values for chosen parameters, and push and pull variables. The only type "
    "of values currently supported is @F {double}. Note that these 'artifical' parameters "
    "and variables are not created until @F initialize() of @F {Fixed} is called. "
    "This is unlike their real counterparts which are always created in the model's constructor");

    addProduct<FunctionalResponseGB>("FunctionalResponseGB", this,
    "The Gutierrez-Baumgaertner functional response model, "
    "including the energy budget for egestion and respiration");

    addProduct<HydroThermalTime>("HydroThermalTime", this,
    "Hydrothermal time accounts for temperature and soil water potential at the same time. "
    "In this implementation the daily increment, as calculated by the @F DayDegrees model, "
    "is set to zero if soil water potential is less than the threshold, otherwise the "
    "daily increment is unaltered. A @F Model named @F {weather} with a pull "
    "variable named @F SWP must exist to supply @F HydroThermalTime with soil water potential.");

    addProduct<Inachis>("Inachis", this,
    "A stage-structured model of the Nymphalid butterfly @I{Inachis io}.");

    AddProduct(Infection,
    "Multi-way infection based upon @F Predation model.");

    addProduct<InsectLifeCycle>("InsectLifeCycle", this,
    "A stage-structured insect model. Under development");

    addProduct<LactinTime>("LactinTime", this,
    "This is the physiological time scale of Lactin et al. (reference to be added). "
    "It pulls the daily average temperature from the @F weather object just like the @F "
    "DayDegrees model. The equation used to calculate the daily increment in physiological "
    "time is @Math{exp(ax) - exp(ab-(b-x) fraction c) + d}, where @I x is daily average "
    "temperature.");

    addProduct<OutputMap>("Map", this,
    "Creates output as an animated map. The @F output element must hold exactly one @F Trace element.");

    addProduct<PhotoThermalTime>("PhotoThermalTime", this,
     "Photothermal time accounts for temperature and day length at the same time. "
     "In this implementation the daily increment, as calculated by the @F DayDegrees model, "
     "is multiplied by day length in hours (acquired from the @F calendar object) "
     "divided by 24 hours.");

    addProduct<OutputPlot>("Plot", this, "Output class. Description pending");

    addProduct<Predation>("Predation", this,
    "Multi-way predation based upon @F FunctionalResponseGB model.");

    addProduct<RandomLognormal>("RandomLognormal", this,
    "The model maintains a random variable with a log-normal distribution.");

    addProduct<RandomNormal>("RandomNormal", this,
    "The model maintains a random variable with a normal distribution.");

    addProduct<RandomPoisson>("RandomPoisson", this,
    "The model maintains a random variable with a Poisson distribution.");

    addProduct<RandomUniform>("RandomUniform", this,
    "The model maintains a random variable with uniform (i.e. flat) distribution.");

    addProduct<Range>("Range", this,
    "The model produces equally spaced numbers inside a [min,max] range.");

    addProduct<Records>("Records", this,
    "A @F Records model reads input from a text file formatted in columns with labels in the first line. "
    "Columns can be separated by spaces or tab characters. Labels may not contain spaces. "
    "A pull variable will be created for every column in the input file, named after the column label. "
    "There can be more than one @F Records model present in a simulation. "
    "@PP Columns named @F date and @F time have special meanings, as they are used to synchronize the "
    "readings from the input file with the @F calendar model. If you have either a @F date or a @F time "
    "column, or both, then the @F calendar model will be reset to the time indicated in the first line of records, "
    "before the simulation begins. "
    "While this is the behaviour usually wanted, you can also change this by setting "
    "@F imposeInitialDateTime to @F {no}. "
    "@PP The pull variables for date and time are mostly used for test purposes. If you "
    "need to know about the current date and time of day, you should pull this information from "
    "the @F calender model. See UniSim::Calendar @CrossLink {page @PageOf UniSim::Calendar}.");

    addProduct<RunIteratorFixed>("RunIteratorFixed", this,
    "The model increments its @F iteration counter for every @F {update}. "
    "Its @F value remains true as long as @Math{@F{iteration <= numIterations}}.");

    addProduct<Scheduled>("Scheduled", this,
    "A @F Scheduled model produces canned data which are derived from a list of time value pairs, e.g., "
    "@F {((10.2 14.6) (45.1 32.3) (57.1 24.43))}. Values are interpolated from the current time "
    "which is taken from the nearest model called @F {time}. "
    "At times outside the range covered by the list (outside [10.2;57.1], in this case) values are extrapolated "
    "(to 14.6 or 24.43, in this case).");

    addProduct<SensitivityAnalysis>("SensitivityAnalysis", this, "Integrator class. Description pending");

    addProduct<Stage>("Stage", this,
    "The @F Stage model implements a distributed delay (@Cite{$label{Manetsch 1976}manetsch, $label{Vansickle 1977}vansickle}). "
    "The implementation follows the original FORTRAN code of @Cite{$label{Abkin \"&\" Wolf (1976)}abkin}. The two parameters @F "
    "duration and @F k determine the average and variance of stage duration with variance equal to "
    "@Math{@F duration sup 2 slash @F k sup 2}. The time scale used by the @F Stage model is "
    "determined by the nearest model called @F {time}.");

    addProduct<StageAndPhase>("StageAndPhase", this,
    "The @F StageAndPhase model simulates a population undergoing two processes at the same, for instance"
    "physiological development (ageing) and incubation (after being infected). "
    "The discern the two, the first is called a stage and the second, a phase. Phase change is characterised"
    "by the parameters @F phaseL and @F phaseK with the same meaning as @F L and @F k in the @F Stage class.");

    addProduct<StageDemand>("StageDemand", this,
    "This model calculates the growth demand of the stage, identified by @F {stage}, which must be of type @F {UniSim::Stage}."
    "The stage's @growthDemand method is used for the calculation.");

    addProduct<Steps>("Steps", this, "Integrator class. Description pending");

    addProduct<Sum>("Sum", this,
        "Calculates sum of variables supplied as a list of references. The variables "
        "must all be of type double.");

    addProduct<OutputTable>("Table", this, "Output class. Description pending");

    addProduct<TimeLimited>("TimeLimited", this, "Integrator class. Description pending");

    addProduct<TriggerByDate>("TriggerByDate", this,
    "The  @F TriggerByDate model can be used by other models to trigger changes in their behaviour.");

    addProduct<Weather>("Weather", this,
    "This @F Weather model simply supplies a daily reading from a @F Records child model. If the records file contains a column named @F Tavg "
    "then that colum is used. Otherwise, if columns named @F Tmin and @F Tmax are present "
    "then @F Tavg is calculated as the average of those");
}

Identifier UniSimFactory::id() const {
    return "UniSim";
}

QString UniSimFactory::description() const {
    return "The @F UniSim plugin contains a collection of models of general utility.";
}

QStringList UniSimFactory::authors() const {
    return QStringList() << "Niels";
}

QObject *UniSimFactory::asQObject() {
    return this;
}

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(unisim_factory, UniSimFactory)
#endif

} //namespace
