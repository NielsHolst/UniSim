/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/variable.h>
#include <usbase/exception.h>
#include "effect.h"

using namespace UniSim;

namespace MicrobialCommunity {

    Effect::Effect(UniSim::Identifier name, QObject *parent) : Model(name, parent) {
        new Parameter<bool>("EnableEffect", &enableEffect, true, this, "Enable the effect");
        new Parameter<QString>("TargetPopulations", &targetPopulations, "", this, "The Population models that are affected under this effect");
    }

    void Effect::initialize() {

        if(enableEffect) {

            populationList = seekSiblings<Population*>("*");

            if(populationList.isEmpty()) {
                throw Exception("No Population model is found for model \"" + this->objectName() +"\". Effect model must be used together with one or more Population models, and they should be placed on the same level in the XML file.", this);
            }

            targetPopulations = targetPopulations.trimmed().toLower(); // e.g. targetPopulations = specified:{bacteria_1, bacteria_2, bacteria_3}

            QRegExp matchAll("\\w+\\:\\{([\\w\\s,.*]+)\\}"); // Matches text like "specified:{bacteria_1, bacteria_2, bacteria_3}" and captures the text within {} into a group.
            matchAll.indexIn(targetPopulations);

            if(matchAll.matchedLength() > 0) {
                matchedPopulations = matchAll.capturedTexts()[1].trimmed(); // e.g. matchedPopulations = "bacteria_1, bacteria_2, bacteria_3"
                matchedPopulationList = matchedPopulations.split(",", QString::SkipEmptyParts); //e.g. paramTargetPopulationList = {"bacteria_1", "bacteria_2", "bacteria_3"}
            } //if

            if(targetPopulations == "all" || targetPopulations == "*") {

                for(int i = 0; i < populationList.size(); i++) {
                    affectedPopulationList.append(populationList[i]);
                }

            } //if "All"

            else if(targetPopulations.startsWith("specified:{")) {

                if(matchAll.matchedLength() > 0) {

                    for(int i = 0; i < populationList.size(); i++) {

                        for(int j = 0; j < matchedPopulationList.size(); j++) {

                            if(populationList[i]->objectName().toLower() == matchedPopulationList[j].trimmed()) {
                                affectedPopulationList.append(populationList[i]);
                                break;
                            }

                        } //for

                    } //for

                } //if
                else throw Exception("\"" + targetPopulations + "\" is not a valid value for parameter TargetPopulations. A valid value should be like \"Specified:{Bacteria_1, Bacteria_2, Bacteria_3}\".", this);

            } //if "Specified"

            else if(targetPopulations.startsWith("allbut:{")) {

                if(matchAll.matchedLength() > 0) {

                    bool isInList = false;
                    for(int i = 0; i < populationList.size(); i++) {

                        for(int j = 0; j < matchedPopulationList.size(); j++) {

                            if(populationList[i]->objectName().toLower() == matchedPopulationList[j].trimmed()) {
                                isInList = true;
                                break;
                            }

                        } //for

                        if(isInList)
                            isInList = false;
                        else
                            affectedPopulationList.append(populationList[i]);

                    } //for

                } //if
                else throw Exception("\"" + targetPopulations + "\" is not a valid value for parameter TargetPopulations. A valid value should be like \"AllBut:{Bacteria_1, Bacteria_2, Bacteria_3}\".", this);

            } //if "AllBut"

            else if(targetPopulations.startsWith("random:{")) {

                if(matchAll.matchedLength() > 0) {

                    bool randomAll = false;

                    if(matchedPopulations == "all" || matchedPopulations == "*") {
                        minPopulations = 1;
                        maxPopulations = populationList.size();
                        randomAll = true;
                    }

                    else {

                        QRegExp matchRange("\\{(\\d+)\\s*,\\s*(\\d+)\\}"); // Matches text like "{2, 10}" and captures each number into a separate group.
                        matchRange.indexIn(matchedPopulations);

                        if(matchRange.matchedLength() > 0) {
                            bool isInt = false;

                            minPopulations = matchRange.capturedTexts()[1].toInt(&isInt);
                            if(!isInt) minPopulations = 1;

                            maxPopulations = matchRange.capturedTexts()[2].toInt(&isInt);
                            if(!isInt) maxPopulations = populationList.size();

                            randomAll = true;
                        }

                        else {

                            for(int i = 0; i < populationList.size(); i++) {

                                for(int j = 0; j < matchedPopulationList.size(); j++) {

                                    if(populationList[i]->objectName().toLower() == matchedPopulationList[j].trimmed()) {
                                        randomPopulationList.append(populationList[i]);
                                        break;
                                    }

                                } //for

                            } //for

                            minPopulations = 1;
                            maxPopulations = randomPopulationList.size();

                        } //else

                    } //else

                    if(randomAll) {

                        for(int i = 0; i < populationList.size(); i++) {
                            randomPopulationList.append(populationList[i]);
                        } //for

                    } //if

                    if(minPopulations == 0) minPopulations = 1;
                    if(minPopulations > randomPopulationList.size()) minPopulations = randomPopulationList.size();
                    if(maxPopulations > randomPopulationList.size()) maxPopulations = randomPopulationList.size();

                    if(minPopulations > maxPopulations) {
                        int temp = minPopulations;
                        minPopulations = maxPopulations;
                        maxPopulations = temp;
                    }

                    reset();

                } //if
                else throw Exception("\"" + targetPopulations + "\" is not a valid value for parameter TargetPopulations. A valid value should be like \"Random:{All}\", \"Random:{Bacteria_1, Bacteria_2, Bacteria_3}\", or \"Random:{0, 5}\".", this);

            } //if "Random"

            else {
                throw Exception("\"" + targetPopulations + "\" is not a valid value for parameter TargetPopulations. Refer to plug-in documentation for correct parameter values.", this);
            }

        } //if(enableEffect)

    } //initialize

    void Effect::reset() {

        if(enableEffect && targetPopulations.startsWith("random:{")) {

            QList<Population*> randomPopulationList_ = randomPopulationList;

            int desiredNum = random.getInt(minPopulations, maxPopulations);
            int activeNum = 0;

            while(desiredNum > activeNum) {

                int i = random.getInt(0, randomPopulationList_.size() - 1);

                // Add the i-th population to the affected population list.
                affectedPopulationList.append(randomPopulationList_[i]);;
                // Remove the added population to ensure each population will only be added once.
                randomPopulationList_.removeAt(i);

                activeNum++;

            } //while

        } //if(enableEffect) && "Random"

    } //reset

} //namespace
