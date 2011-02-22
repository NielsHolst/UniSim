/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include "population.h"

using namespace UniSim;

namespace MicrobialCommunity {

    Population::Population(UniSim::Identifier name, QObject *parent)
        : Model(name, parent) {
        new Parameter<QString>("SpeciesName", &speciesName, QString("Bacterium"), this, "Name of the microbial species");

        new Parameter<bool>("IsActive", &isActive, true, this, "A switch indicating whether this population is active in a stochastic community.");
        new PushVariable<bool>("IsActive", &isActive, this, "");

        new Parameter<double>("InitialDensity", &initialDensity, 0., this, "Initial density");
        new PushVariable<double>("InitialDensity", &initialDensity, this, "Initial density");

        new Parameter<double>("CarryingCapacity", &carryingCapacity, 0., this, "Carrying capacity");
        new PushVariable<double>("CarryingCapacity", &carryingCapacity, this, "Carrying capacity");

        new Parameter<double>("LagPhase", &lagPhase, 0., this, "Lag phase");
        new PushVariable<double>("LagPhase", &lagPhase, this, "Lag phase");

        new Parameter<double>("GrowthRate", &growthRate, 0., this, "Growth rate");
        new PushVariable<double>("GrowthRate", &growthRate, this, "Growth rate");

        new Parameter<double>("MortalityRate", &mortalityRate, 0., this, "Mortality rate");
        new PushVariable<double>("MortalityRate", &mortalityRate, this, "Mortality rate");

        new Parameter<double>("SearchRate", &searchRate, 0., this, "Search rate");
        new PushVariable<double>("SearchRate", &searchRate, this, "Search rate");

        new Parameter<double>("DemandRate", &demandRate, 0., this, "Demand rate");
        new PushVariable<double>("DemandRate", &demandRate, this, "Demand rate");

        new Parameter<double>("ZeroOrderRateConstant", &rateConstant_Zero, 0., this, "Zero-order rate constant");
        new PushVariable<double>("ZeroOrderRateConstant", &rateConstant_Zero, this, "Zero-order rate constant");

        new Parameter<double>("FirstOrderRateConstant", &rateConstant_First, 0., this, "First-order rate constant");
        new PushVariable<double>("FirstOrderRateConstant", &rateConstant_First, this, "First-order rate constant");

        new Parameter<double>("MichaelisMentenConstant", &rateConstant_Km, 0., this, "Michaelis-Menten constant (Km) for the enzyme");
        new PushVariable<double>("MichaelisMentenConstant", &rateConstant_Km, this, "Michaelis-Menten constant (Km) for the enzyme");

        new Parameter<double>("MaximumVelocity", &rateConstant_Vmax, 0., this, "Maximum velocity of the enzyme-catalyzed reaction");
        new PushVariable<double>("MaximumVelocity", &rateConstant_Vmax, this, "Maximum velocity of the enzyme-catalyzed reaction");

        new PullVariable<double>("Density", &density, this, "Population density");
        new PullVariable<double>("DensityActivityCoefficient", &densityActivityCoef, this, "Bacterial density-activity coefficient");

        new PullVariable<double>("SupplyDemandRatio", &sdRatio, this, "Supply-demand ratio");
        new PushVariable<double>("SupplyDemandRatio", &sdRatio, this, "Supply-demand ratio");

    }

    void Population::initialize() {

        calendar = seekOne<Model*>("Calendar");

        // Reset values
        reset();

    }

    void Population::reset() {

        //isActive = true;

        densityActivityCoef = 1;
        sdRatio = 1;

        if(isActive)
            density = initialDensity;
        else
            density = 0;

    }

    void Population::update() {

        double time = calendar->pullVariable<double>("totalTime");

        if(time > lagPhase && density != 0) {

            double dDensity = carryingCapacity <= 0 ? (density * (growthRate * sdRatio - mortalityRate)) : (density * (growthRate * (1 - density / carryingCapacity) - mortalityRate));
            // Note here that sdRatio will only be incorporated when CarryingCapacity <= 0. When a Competition model is included, it will set CarryingCapacity to 0 at initialization.

            densityActivityCoef = (density == 0 ? 1 : (1 + dDensity / density));

            density += dDensity;

            if(density < 1) density = 0;
        }

     }

} //namespace
