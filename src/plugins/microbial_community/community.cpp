/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include <usbase/exception.h>
#include <QtAlgorithms>
#include <QRegExp>
#include "community.h"

using namespace UniSim;

namespace MicrobialCommunity {

    Community::Community(UniSim::Identifier name, QObject *parent)
        : Model(name, parent) {
        new Parameter<bool>("EnableStochasticCommunity", &enableStochasticCommunity, false, this, "A switch indicating whether stochastic community is enabled");
        new Parameter<int>("MinPopulations", &minPopulations, 1, this, "Minimum number of populations entered in the stochastic community");
        new Parameter<int>("MaxPopulations", &maxPopulations, 1, this, "Maximum number of populations entered in the stochastic community");
        new Parameter<QString>("AlwaysIncludedPopulations", &alwaysIncluded, "", this, "A list of populations that will always be entered in the stochastic community");

        new Variable<double>("Density", &density, this, "Total density of the community");

        new Variable<int>("Richness", &richness, this, "Community richness");
        new Variable<double>("SimpsonIndex", &simpsonIndex, this, "Simpson's Index");
        new Variable<double>("ShannonIndex", &shannonIndex, this, "Shannon-Wiener Index");
        new Variable<double>("Evenness", &evenness, this, "Community evenness");
        new Variable<double>("GiniCoefficient", &giniCoeff, this, "Gini coefficient");
    }

    void Community::initialize() {
        populationList = seekChildren<Population*>("*");

        if(populationList.isEmpty()) {
            throw Exception("No Population model is found inside community model \"" + this->objectName() + "\". Community model must contain one or more Population models.", this);
        }

        if(minPopulations > populationList.size()) minPopulations = populationList.size();
        if(maxPopulations > populationList.size()) maxPopulations = populationList.size();

        if(minPopulations > maxPopulations) {
            int temp = minPopulations;
            minPopulations = maxPopulations;
            maxPopulations = temp;
        }

        alwaysIncludedPopulationList = alwaysIncluded.toLower().split(",", QString::SkipEmptyParts);

        if(enableStochasticCommunity) {

            // Split populations into required & optional. This only need to be performed once so it's placed in initialize().
            for(int i = 0; i < populationList.size(); i++) {

                bool found = false;
                for(int j = 0; j < alwaysIncludedPopulationList.size(); j++) {

                    // Find whether the population is marked "Always Included".
                    if(populationList[i]->objectName().toLower().trimmed() == alwaysIncludedPopulationList[j].trimmed()) {
                        requiredPopulationList.append(populationList[i]);
                        found = true;
                        break;
                    }

                } //for

                if(!found) {
                    optionalPopulationList.append(populationList[i]);
                }
            } //for

        } //if(enableStochasticCommunity)


    } //initialize

    void Community::reset() {

        if(enableStochasticCommunity) {

            // Create temporary copies of required and optional population list. This protects the original list from being modified.
            QList<Population*> requiredPopulationList_ = requiredPopulationList;
            QList<Population*> optionalPopulationList_ = optionalPopulationList;

            int desiredNum = random.getInt(minPopulations, maxPopulations);
            int activeNum = 0;

            for(int i = 0; i < populationList.size(); i++) {
                // Deactivate all Population models to allow selective enabling.
                populationList[i]->pushValue<bool>("IsActive", false);
            }

            if(desiredNum >= requiredPopulationList_.size()) {

                if(requiredPopulationList_.size() > 0) {
                    // First include all required populations.
                    for(int i = 0; i < requiredPopulationList_.size(); i++) {
                        requiredPopulationList_[i]->pushValue<bool>("IsActive", true);
                        activeNum++;
                    }
                }

                // For the remainder, add optional populations.
                while(desiredNum > activeNum) {

                    int i = random.getInt(0, optionalPopulationList_.size() - 1);

                    // Add the i-th population to the optional list.
                    optionalPopulationList_[i]->pushValue<bool>("IsActive", true);
                    // Remove the added population to ensure each population will only be added once.
                    optionalPopulationList_.removeAt(i);

                    activeNum++;

                } //while

            } //if
            else {

                // Select randomly from required populations.
                while(desiredNum > activeNum) {

                    int i = random.getInt(0, requiredPopulationList_.size() - 1);
                    requiredPopulationList_[i]->pushValue<bool>("IsActive", true);
                    requiredPopulationList_.removeAt(i);
                    activeNum++;

                } //while

            } //else

        } //if(enableStochasticCommunity)

        update();

    } //reset

    void Community::update() {

        density = 0.;
        richness = 0;
        simpsonIndex = 0.;
        shannonIndex = 0.;
        evenness = 0.;
        giniCoeff = 0;

        QList<double> popDensity;

        for(int i = 0; i < populationList.size(); i++) {
            // Calculate total density of the community.
            density += populationList[i]->pullValue<double>("Density"); // This gives total density.
        }

        for(int i = 0; i < populationList.size(); i++) {

            double populationDensity = populationList[i]->pullValue<double>("Density");

            popDensity.append(populationDensity);

            if(populationDensity > 0.) {
                richness += 1; // This gives Richness.
            }

            simpsonIndex += populationDensity * (populationDensity - 1);  // This is only part of the calculation, not the real Simpson's Index.

            if(populationDensity > 0 && density > 0) {
                double relativeAbundance = (populationDensity / density);
                shannonIndex -= relativeAbundance * log(relativeAbundance); // This gives Shannon-Wiener Index.
            }

        } //for

        simpsonIndex = 1 - simpsonIndex / (density * (density - 1)); // This gives the final Simpson's Index.

        if(richness > 0) {
            evenness = shannonIndex / log(richness); // This gives Evenness Index.
        }

        qSort(popDensity);

        if(richness > 0) {

            double Sum_iYi = 0;
            for(int i = 0; i < richness; i++) {
                Sum_iYi += (i + 1) * popDensity[i];
            }

            giniCoeff = (2 * Sum_iYi) / (richness * density) - (richness + 1) / richness;
        }

     } //update

} //namespace
