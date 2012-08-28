/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "deposition_flush.h"
#include "deposition_monotonic_sampled.h"
#include "deposition_monotonic_weibull.h"
#include "ecotox_factory.h"
#include "log_logistic.h"
#include "pollen.h"
#include "pollen_loss_constant.h"
#include "pollen_loss_sampled.h"

using namespace UniSim;

namespace ecotox{

void EcotoxFactory::defineProducts() {
    addProduct<DepositionFlush>("DepositionFlush", this,
    "With this model deposition comes in a flush. The daily deposition rate follows a parabolic curve during the deposition period. "
    "The flush starts on a date sampled randomly from a seasonal deposition curve, which must be defined in a "
    "sibling model called @F {depositionRate}. This can, for example, be one of the models @F DepositionMonotonicSampled "
    "or @F {DepositionMonotonicWeibull}.");

    addProduct<DepositionMonotonicSampled>("DepositionMonotonicSampled", this,
    "Seasonal deposition curve sampled from an empirical distribution, beginning at value zero and ending at value one "
    "at the end of the season. The distribution data must be supplied as a column in the records of the @F weather model.");

    addProduct<DepositionMonotonicWeibull>("DepositionMonotonicWeibull", this,
    "Seasonal deposition curve described by a Weibull curve");

    addProduct<LogLogistic>("LogLogistic", this,
    "The log-logistic dose-response curve described by @PP @PP\n"
    "@Math{y = y sub min - {(y sub max - y sub min)} over {1 + {x over ED50} sup {-slope}} } \n@LP @LP\n"
    "The value of the dose @I x is given by the parameter @F dose below. As for any other parameter, "
    "you can either set this to a fixed or a variable value. For @F dose you most often want a "
    "variable value supplied from another model. If you set @F dose to @F pollen[pollenDensity] then "
    "the response will reflect current pollen density. Other possible measures include "
    "@F pollen[stdPollenDensity] and @F {pollen[toxinDensity]}, "
    "see {ecotox::Pollen @CrossLink {@F Pollen on page @PageOf ecotox::Pollen}}.");

    addProduct<Pollen>("Pollen", this,
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
    "@F{stdPollenDensity = @PP\npollenDensity*pollenMass/stdPollenMass*toxinConc/stdToxinConc} @LP\n");

    addProduct<PollenLossConstant>("PollenLossConstant", this,
    "With a constant rate of pollen loss, pollen disappears following a negetive exponential curve.");

    addProduct<PollenLossSampled>("PollenLossSampled", this,
    "With sampled loss rate, the daily loss rate is determind by picking a value at random from the supplied list of loss rates.");
}

UniSim::Identifier EcotoxFactory::id() const {
    return "ecotox";
}

QString EcotoxFactory::description() const {
    return
        "A collection of models to simulate deposition of a substance around a source, "
        "for instance, GM pollen or a pesticide. The current naming of models implies "
        "GM pollen";
}

QStringList EcotoxFactory::authors() const {
    return QStringList()
        << "Niels";
}

QObject* EcotoxFactory::asQObject() {
   return this;
}

Q_EXPORT_PLUGIN2(ecotox_factory, EcotoxFactory)

} //namespace
