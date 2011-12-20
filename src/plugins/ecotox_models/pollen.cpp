/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/parameter.h>
#include <usbase/pull_variable.h>
#include "pollen.h"

using namespace UniSim;


namespace ecotox {

Pollen::Pollen(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    new Parameter<double>("distance", &distance, 0.3, this,
                          "Distance to maize field (m)");
    new Parameter<double>("minDist", &minDist, 0.3, this,
                          "Minimum distance of pollen dispersion model (m)");
    new Parameter<double>("halfDist", &halfDist, 3.55, this,
                          "Pollen deposition declines as a negative exponential with distance from the maize field. "
                          "@F halfDist denotes the distance (m) at which deposition is halfed");
    new Parameter<double>("maxTotal", &maxTotal, 120., this,
                          "Maximum of total pollen deposited on food plant (pollen per cm @Sup {2})."
                          "This occurs at @F minDist distance from the maize field");

    new Parameter<double>("pollenMass", &pollenMass, 0.25, this,
                          "Fresh weight of one pollen grain (@Sym{mu}g)");
    new Parameter<double>("stdPollenMass", &stdPollenMass, 0.25, this,
                          "Standard fresh weight of one pollen grain (@Sym{mu}g)");
    new Parameter<double>("toxinConc", &toxinConc, 0.01, this,
                          "Toxin concentration in pollen (@Sym{mu}g/g fresh weight)");
    new Parameter<double>("stdToxinConc", &stdToxinConc, 0.01, this,
                          "Standard toxin concentration in pollen (@Sym{mu}g/g fresh weight)");

    new PullVariable<double>("pollenDepositionRate", &pollenDepositionRate, this,
                             "Current pollen deposition rate (per day per cm @Sup {2})");
    new PullVariable<double>("pollenDepositionTotal", &pollenDepositionTotal, this,
                             "Total pollen deposition (per cm @Sup {2})");
    new PullVariable<double>("pollenDensity", &pollenDensity, this,
                             "Current pollen density (per cm @Sup {2})");
    new PullVariable<double>("toxinDensity", &toxinDensity, this,
                             "Current toxin density (@Sym{mu}g per cm @Sup {2})");
    new PullVariable<double>("stdPollenDensity", &stdPollenDensity, this,
                             "Current pollen density, "
                             "expressed as standard pollen correct for mass and toxin concentration "
                             "(per cm @Sup {2})");
}

void Pollen::initialize() {
    Model *flush = peekOneChild<Model*>("depositionFlush");
    if (flush) {
        depositionRate = flush->pullVariablePtr<double>("value");
    }
    else {
        Model *deposition = seekOneChild<Model*>("depositionRate");
        depositionRate = deposition->pullVariablePtr<double>("value");
    }
    Model *loss = seekOneChild<Model*>("lossRate");
    lossRate = loss->pullVariablePtr<double>("value");
}

void Pollen::reset() {
    pollenDepositionRate =
    pollenDepositionTotal =
    pollenDensity =
    toxinDensity =
    stdPollenDensity = 0.;
    PdistRate = log(2)/log(halfDist);
}

void Pollen::update() {
    pollenDepositionRate = Pdist(distance)*(*depositionRate);
    pollenDepositionTotal += pollenDepositionRate;
    pollenDensity *= 1. - (*lossRate);
    pollenDensity +=  pollenDepositionRate;
    toxinDensity = pollenDensity*pollenMass*toxinConc;
    stdPollenDensity = pollenDensity*pollenMass/stdPollenMass*toxinConc/stdToxinConc;
}

double Pollen::Pdist(double distance) {
    return maxTotal*pow(distance, PdistRate)/pow(minDist, PdistRate);
}
} //namespace

