/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <QString>
#include <usbase/component.h>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include <usbase/utilities.h>
#include "rotation.h"
#include "weed.h"

using namespace UniSim;

namespace awe {

Weed::Weed(UniSim::Identifier name, QObject *parent)
    : Model(name, parent)
{
    setRecursionPolicy(Component::Update, Component::ChildrenNot);

    new Parameter<double>("c1", &c1, 6.904, this,
                 "Parameter in the equation that calculates density equivalents from density, "
                 "@Math{y = exp braceleft c sub 1 bracketleft exp(-{x over c sub 2}) - 1 bracketright braceright "
                 "- exp(-c sub {1})}, "
                 "where @I x is crop leaf area index");
    new Parameter<double>("c2", &c2, 0.7491, this,
                 "See @F {c1}");
    new Parameter<double>("seedProdSlope", &seedProdSlope, 2., this,
                 "Slope @I a (seeds per g @Sup {b}) in the equation relating seed production to final weed biomass, "
                 " @Math{y = ax sup b }"
                 "where @I x is weed biomass (g)");
    new Parameter<double>("seedProdExp", &seedProdExp, 1., this,
                 "Exponent @I b in the equation relating seed production to final weed biomass, "
                 " @Math{y = ax sup b }"
                 "where @I x is weed biomass (g)");

    new PullVariable<double>("projectedDeqs", &projectedDeqs, this,
                     "The projected number of density equivalents (per m @Sup {2}) is the final density equivalents expected, "
                     "if there will be no mortality applied to any growth stages");
    new PullVariable<double>("projectedYieldLossPct", &projectedYieldLossPct, this,
                     "The projected yield loss (%) is calculated from @F projectedDeqs by way of the Cousens equation");
    new PullVariable<double>("projectedMass", &projectedMass, this,
                     "The projected weed biomass (g\"/\"m @Sup {2}) is calculated from @F projectedYieldLossPct "
                     "applying the weed\"/\"yield biomass exchange rate pertinent to the @F {Crop}");
    new PullVariable<double>("seedsDropping", &seedsDropping, this,
                     "The number of seeds (per m @Sup 2 per day) dropping to the ground during this time step. "
                     "These originate from the @F seedsOnPlant model");
    new PullVariable<double>("g", &g, this,
                     "Current seed multiplication rate for the @F seedsOnPlant model. "
                     "Works by unpublished mechanism");
}

void Weed::initialize() {
    seedBank = seekOneChild<Model*>("seedBank");
    seedling = seekOneChild<Model*>("seedling");
    juvenile = seekOneChild<Model*>("juvenile");
    mature   = seekOneChild<Model*>("mature");
    seedsOnPlant = seekOneChild<Model*>("seedsOnPlant");

    calendar = seekOne<Model*>("calendar");
    rotation = seekOne<Rotation*>("rotation");
    weather = seekOne<Model*>("weather");

    if (!rotation) throw Exception("Weed could not find rotation model");

    QList<Model*> crops = rotation->cropModels();
    for (int i = 0; i < crops.size(); ++i)
        connect(crops[i], SIGNAL(event(QObject*, QString)), this, SLOT(handleEvent(QObject*, QString)));
}

void Weed::reset() {
    projectedDeqs = prevOutflowTotal = seedsDropping = 0;
}

void Weed::update() {
    seedBank->pushVariable("dormantInflow", seedsDropping);
    seedsDropping = 0;
    seedBank->update();
    double newSeedlings = seedBank->pullVariable<double>("dailyEmergenceDensity");

    seedling->pushVariable("inflowAsDensity", newSeedlings);
    seedling->pushVariable("inflowAsDensityEqs", newSeedlings*cropEffectOnSeedlings());
    seedling->update();

    juvenile->pushVariable("inflowAsDensity",
                            seedling->pullVariable<double>("outflowAsDensity"));
    juvenile->pushVariable("inflowAsDensityEqs",
                            seedling->pullVariable<double>("outflowAsDensityEqs"));
    juvenile->update();

    mature->pushVariable("inflowAsDensity",
                          juvenile->pullVariable<double>("outflowAsDensity"));
    mature->pushVariable("inflowAsDensityEqs",
                          juvenile->pullVariable<double>("outflowAsDensityEqs"));
    mature->update();

	projectCompetitionOutcome();
    double seedInflowAsMass = (seedProdSlope > 0.) ? proportionDeqsEnteringMaturity()*projectedMass : 0.;

    g = (projectedMass > 1e-6) ?  seedProdSlope*pow(projectedMass, seedProdExp)/projectedMass : 1.;

    seedsOnPlant->pushVariable("inflow", seedInflowAsMass);
    if (g > 0) seedsOnPlant->pushVariable("growthRate", g);
    seedsOnPlant->deepUpdate();

    seedsDropping += seedsOnPlant->pullVariable<double>("outflow");
}

double Weed::cropEffectOnSeedlings() const {
    double lai = rotation->pullVariable<double>("lai");
    return exp(c1*(exp(-lai/c2)-1.)) - exp(-c1);
}

void Weed::projectCompetitionOutcome() {
    projectedDeqs = calcProjectedDeqs();
    projectedYieldLossPct = yieldLossPct(projectedDeqs);

    Model *crop = rotation->currentCrop();
    double maxYield = crop->parameter<double>("maxYield"),
           weedExchangeRate = crop->parameter<double>("weedExchangeRate");

    projectedMass = maxYield*projectedYieldLossPct/weedExchangeRate/100.;
}

double Weed::calcProjectedDeqs() {
    Model *seedlingDEqs = seedling->seekOneChild<Model*>("densityEqs"),
          *juvenileDEqs = juvenile->seekOneChild<Model*>("densityEqs"),
          *matureDEqs   = mature->seekOneChild<Model*>("densityEqs");

    return seedlingDEqs->pullVariable<double>("number") +
           juvenileDEqs->pullVariable<double>("number") +
           matureDEqs->pullVariable<double>("number") +
           matureDEqs->pullVariable<double>("outflowTotal") -
           prevOutflowTotal;
}

double Weed::yieldLossPct(double N) const
{
  Model *crop = rotation->currentCrop();
  double a = crop->parameter<double>("maxYieldLossPct"),
         i = crop->parameter<double>("slopeYieldLossPct");

  return (a == 0) ? 0. : i*N/(1. + i*N/a);;
}

double Weed::proportionDeqsEnteringMaturity() const {
    double prop = (projectedDeqs > 0) ?
                  juvenile->pullVariable<double>("outflowAsDensityEqs")/projectedDeqs : 0.;

	if (prop > 1.){
		//Q_ASSERT(prop < 1.001);
		prop = 1.;
	}
	return prop;
}

void Weed::handleEvent(QObject *sender, QString event) {
    if (event == "sowing" || event == "harvest") {
        kill(seedling, 100);
        kill(juvenile, 100);
        kill(mature, 100);
        kill(seedsOnPlant, 100);

        Model *matureDeqs = mature->seekOneChild<Model*>("densityEqs");
        prevOutflowTotal = matureDeqs->pullVariable<double>("outflowTotal");
    }
}

void Weed::kill(Model *stage, double mortalityPct) {
    stage->pushVariable("instantMortality", mortalityPct);
}


}
