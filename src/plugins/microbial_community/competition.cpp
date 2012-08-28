/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include <usbase/exception.h>
#include "competition.h"

using namespace UniSim;

namespace MicrobialCommunity {

    Competition::Competition(UniSim::Identifier name, QObject *parent)
        : Model(name, parent) {
        new Parameter<bool>("EnableCompetition", &enableCompetition, true, this, "A switch indicating whether competition model is enabled");
        new Parameter<double>("InitialResource", &initialResource, 1., this, "Initial resource");
        new Parameter<QString>("CompetitionModel", &competitionModel, "default", this, "Define the competition model used in the simulation");

        new Variable<double>("AvailableResource", &availableResource, this, "Description");

    }

    void Competition::initialize() {

        populationList = seekSiblings<Population*>("*");

        if(populationList.isEmpty()) {
            throw Exception("No Population model is found for model \"" + this->objectName() +"\". Competition model must be used together with one or more Population models, and they should be placed on the same level in the XML file.", this);
        }

        for(int i = 0; i < populationList.size(); i++) {
            // For Competition model, CarryingCapacity is not required and should be set to 0.
            populationList[i] -> pushValue<double>("CarryingCapacity", 0);
        }

        // Call reset function to set parameter values.
        reset();

    } //initialize

    void Competition::reset() {
        availableResource = initialResource;
    } //reset

    void Competition::update() {

        if(enableCompetition) {

            // It is possible here to incorporate different competition models.

            if(competitionModel.toLower() == "default") {
                runCompetitionModel();
            }
            else {
                runCompetitionModel();
            }

        } //if

     } //update

    void Competition::runCompetitionModel() {

        QList<double> demand; // demand = Density * DemandRate * dTime
        QList<double> consumption; // consumption = demand * (1 - EXP(-SearchRate * AvailableResource / demand))
        double totalConsumption = 0;

        for(int i = 0; i < populationList.size(); i++){
            // This loop calculates individual demand, comsumption rate & total consumption rate.

            double density = populationList[i]->pullValue<double>("Density");
            double demandRate = populationList[i]->pullValue<double>("DemandRate");
            double searchRate = populationList[i]->pullValue<double>("SearchRate");

            demand.append(density * demandRate);

            consumption.append(demand[i] == 0 ? 0 : (demand[i] * (1 - exp(-searchRate * availableResource / demand[i])))); // Note: if availableResource = 0, consumption = 0.
            totalConsumption += consumption[i];

        }

        if(totalConsumption <= availableResource) {
            availableResource -= totalConsumption;
        }
        else {
            // There's less resource than consumption, resource is exhausted by being divided among consumers in proportion to individual consumption.
            for(int i = 0; i < populationList.size(); i++) {
                consumption[i] = consumption[i] / totalConsumption * availableResource;
            }
            availableResource = 0;
        }

        for(int i = 0; i < populationList.size(); i++) {
            // This loop calculates individual supply-demand ratio and return the result to the population model.

            double sdRatio = (demand[i] == 0 ? 0 : consumption[i] / demand[i]);

            populationList[i] ->pushValue<double>("SupplyDemandRatio", sdRatio); // Population growth rate is ratio-dependent.
        }

    }

} //namespace
