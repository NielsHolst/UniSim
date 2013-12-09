/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "ecotox_factory.h"
#include "log_logistic.h"
#include "pollen.h"
#include "pollen_onset_date_from_maize_obs.h"
#include "pollen_onset_date_from_trap_obs.h"

using namespace UniSim;

namespace ecotox{

void EcotoxFactory::defineProducts() {

    AddProduct(LogLogistic,
    "The log-logistic dose-response curve described by @PP @PP\n"
    "@Math{y = y sub min - {(y sub max - y sub min)} over {1 + {x over ED50} sup {-slope}} } \n@LP @LP\n"
    "The value of the dose @I x is given by the parameter @F dose below. As for any other parameter, "
    "you can either set this to a fixed or a variable value. For @F dose you most often want a "
    "variable value supplied from another model. If you set @F dose to @F pollen[pollenDensity] then "
    "the response will reflect current pollen density. Other possible measures include "
    "@F pollen[stdPollenDensity] and @F {pollen[toxinDensity]}, "
    "see {ecotox::Pollen @CrossLink {@F Pollen on page @PageOf ecotox::Pollen}}.");

    AddProduct(Pollen,
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

    AddProduct(PollenOnsetDateFromMaizeObs,
    "The onset of pollen deposition is sampled from a file with at least two columns named @F Date and @F Onset. "
    "A date is picked at random from within the observations of the current year (determined by the @F Calendar model).");

    AddProduct(PollenOnsetDateFromTrapObs,
    "The onset of pollen deposition is sampled from the accumulated maize pollen catch from a pollen trapping station. "
    "The pollen trap data is read from a file with at least two columns named @F Date and @F Pollen. "
    "The pollen data is given as the relative trap catch over the year. Hence the column should add up to 1 for every year in the file. ");
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

#if QT_VERSION < 0x50000
Q_EXPORT_PLUGIN2(ecotox_factory, EcotoxFactory)
#endif

} //namespace
