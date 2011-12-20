/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/object_pool.h>
#include <usbase/utilities.h>
#include "deposition_flush.h"
#include "deposition_monotonic_sampled.h"
#include "deposition_monotonic_weibull.h"
#include "ecotox_model_maker.h"
#include "log_logistic.h"
#include "pollen.h"
#include "pollen_loss_constant.h"
#include "pollen_loss_sampled.h"

using namespace UniSim;

namespace ecotox{

QString EcotoxModelMaker::pluginDesc() const {
    return
            "A collection of models to simulate deposition of a substance around a source, "
            "for instance, GM pollen or a pesticide. The current naming of models implies "
            "GM pollen";
}

Identifier EcotoxModelMaker::pluginName() const {
	return "ecotox";
}

QStringList EcotoxModelMaker::authors() const
{
    return QStringList() << "Niels" << "Frieder";
}

const QMap<Identifier, QString>& EcotoxModelMaker::supportedClasses()
{
	if (!desc.isEmpty()) return desc;

    desc["DepositionFlush"] =
            "With this model deposition comes in a flush. The daily deposition rate follows a parabolic curve during the deposition period. "
            "The flush starts on a date sampled randomly from a seasonal deposition curve, which must be defined in a "
            "sibling model called @F {depositionRate}. This can, for example, be one of the models @F DepositionMonotonicSampled "
            "or @F {DepositionMonotonicWeibull}.";

    desc["DepositionMonotonicSampled"] =
            "Seasonal deposition curve sampled from an empirical distribution, beginning at value zero and ending at value one "
            "at the end of the season. The distribution data must be supplied as a column in the records of the @F weather model.";

    desc["DepositionMonotonicWeibull"] =
            "Seasonal deposition curve described by a Weibull curve";

    desc["LogLogistic"] =
            "The log-logistic dose-response curve described by @PP @PP\n"
            "@Math{y = y sub min - {(y sub max - y sub min)} over {1 + {x over ED50} sup {-slope}} } \n@LP @LP\n"
			"The value of the dose @I x is given by the parameter @F dose below. As for any other parameter, "
			"you can either set this to a fixed or a variable value. For @F dose you most often want a "
			"variable value supplied from another model. If you set @F dose to @F pollen[pollenDensity] then "
			"the response will reflect current pollen density. Other possible measures include "
            "@F pollen[stdPollenDensity] and @F {pollen[toxinDensity]}, "
            "see {ecotox::Pollen @CrossLink {@F Pollen on page @PageOf ecotox::Pollen}}.";

    desc["Pollen"] =
            "This model must be composed of two models, @F depositionRate and @F {lossRate}, "
            "and possibly a third model, @F {depositionFlush}. Its parameters determine the total deposition "
            "and the distance from the source. @LP\n"
            "The @F LogLogistic dose-response model can pull the current dose from @F Pollen as "
            "@F{pollenDensity}, @F{toxinDensity} or @F{stdPollenDensity}."
            "The exchange rate between these different units of density is determined by the parameters "
            "@F pollenMass and @F{toxinConc}, which set the mass and toxin concentration for the crop at hand, "
            "and @F stdPollenMass and @F{stdToxinConc}, which set the corresponding values for a standard "
            "reference crop. The transformations are @PP\n"
            "@F{toxinDensity = pollenDensity*pollenMass*toxinConc} @LP\n"
            "and  @PP\n"
            "@F{stdPollenDensity = @PP\npollenDensity*pollenMass/stdPollenMass*toxinConc/stdToxinConc} @LP\n";

    desc["PollenLossConstant"] =
            "With a constant rate of pollen loss, pollen disappears following a negetive exponential curve.";

    desc["PollenLossSampled"] =
            "With sampled loss rate, the daily loss rate is determind by picking a value at random from the supplied list of loss rates.";

    return desc;
}

Model* EcotoxModelMaker::create(Identifier modelType, Identifier objectName, QObject *parent)
{
	setSimulationObjectFromDescendent(parent);
	Model *model = 0;
    if (modelType.equals("DepositionFlush"))
        model = new DepositionFlush(objectName, parent);
    else if (modelType.equals("DepositionMonotonicSampled"))
        model = new DepositionMonotonicSampled(objectName, parent);
    else if (modelType.equals("DepositionMonotonicWeibull"))
        model = new DepositionMonotonicWeibull(objectName, parent);
    else if (modelType.equals("LogLogistic"))
        model = new LogLogistic(objectName, parent);
    else if (modelType.equals("Pollen"))
		model = new Pollen(objectName, parent);
    else if (modelType.equals("PollenLossConstant"))
        model = new PollenLossConstant(objectName, parent);
    else if (modelType.equals("PollenLossSampled"))
        model = new PollenLossSampled(objectName, parent);
    return model;
}


Q_EXPORT_PLUGIN2(ecotox_model_maker, EcotoxModelMaker)

} //namespace
