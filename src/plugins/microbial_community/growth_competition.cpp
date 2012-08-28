/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include "growth_competition.h"

using namespace UniSim;

namespace MicrobialCommunity {

    GrowthCompetition::GrowthCompetition(UniSim::Identifier name, QObject *parent)
        : Model(name, parent) {
        new Parameter<double>("CarryingCapacity", &carryingCapacity, 100., this, "Carrying capacity of the community");
        new Parameter<double>("InitialResource", &initialResource, 1., this, "Initial resource");
        new Variable<double>("AvailableResource", &availableResource, this, "Description");
        new Variable<double>("TotalDensity", &totalDensity, this, "Description");
    }

    void GrowthCompetition::initialize() {
        Timeline = seekOne<Model*>("Timeline");
        Populations = seekMany<Population*>("*");

        // Call reset function to set parameter values.
        reset();
    }

    void GrowthCompetition::reset() {

        availableResource = initialResource;
        updateDensity();

    }

    void GrowthCompetition::update() {

        QList<double> demand; // demand = Density * DemandRate * dTime
        QList<double> consumption; // consumption = demand * (1 - EXP(-SearchRate * AvailableResource / demand))
        double totalConsumption = 0;

        for(int i = 0; i < Populations.size(); i++){
            // This loop calculates individual demand, comsumption rate & total consumption rate.

            double density = Populations[i] -> pullValue<double>("Density");
            double demandRate = Populations[i] -> parameter<double>("DemandRate");
            double searchRate = Populations[i] -> parameter<double>("SearchRate");

            demand.append(density * demandRate);

            consumption.append(demand[i] == 0 ? 0 : (demand[i] * (1 - exp(-searchRate * availableResource / demand[i])))); // Note: if availableResource = 0, consumption = 0.
            totalConsumption += consumption[i];

        }

        if(totalConsumption <= availableResource) {
            availableResource -= totalConsumption;
        }
        else {
            // There's less resource than consumption, resource is exhausted by being divided among consumers in proportion to individual consumption.
            for(int i = 0; i < Populations.size(); i++) {
                consumption[i] = consumption[i] / totalConsumption * availableResource;
            }
            availableResource = 0;
        }

        for(int i = 0; i < Populations.size(); i++) {
            // This loop calculates individual supply-demand ratio and return the result to the population model.

            double sdRatio = (demand[i] == 0 ? 0 : consumption[i] / demand[i]);

            Populations[i] ->pushValue("SupplyDemandRatio", sdRatio); // Population growth rate is ratio-dependent.
        }

       updateDensity();

     }

    void GrowthCompetition::updateDensity() {
        // This function retrieves individual population density from each population and calculates the total density for the community.

        totalDensity = 0;

        for(int i = 0; i < Populations.size(); i++){
            double density = Populations[i] -> pullValue<double>("Density");
            totalDensity += density;
        }

    }

} //namespace
