/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "infection.h"
#include "predation.h"

namespace UniSim {

Infection::Infection(Identifier name, QObject *parent)
    : FunctionalResponseMultiway(name, parent)
{
}

void Infection::createVariables() {
    attacked.resize(numPredators, numPrey);
    propAttacked.resize(numPredators, numPrey);

    attacksVector.resize(numPredators);
    attackedVector.resize(numPredators);
    attacksHost.resize(numPrey);
    attackedHost.resize(numPrey);

    for (int i = 0; i < numPredators; ++i) {
        QString predatorName = predatorNames[i].replace("/","_");
        for (int j = 0; j < numPrey; ++j) {
            QString preyName = preyNames[j].replace("/","_");
            QString name = QString("%1-%2-").arg(predatorName).arg(preyName);
            addVariable<double>(name+"Attacks", &attacks(i,j), "Number of attacks");
            addVariable<double>(name+"Attacked", &attacked(i,j), "Number of hosts attacked");
            addVariable<double>(name+"PropAttacked", &propAttacked(i,j), "Proportion of hosts attacked");
        }
        QString nameAttacks = QString("%1-Attacks").arg(predatorName);
        addVariable<double>(nameAttacks, &attacksVector[i], "Total number of attacks by vector");
        QString nameAttacked = QString("%1-Attacked").arg(predatorName);
        addVariable<double>(nameAttacked, &attackedVector[i], "Total number of hosts attacked by vector");
    }

    for (int j = 0; j < numPrey; ++j) {
        QString hostName = preyNames[j];
        QString nameAttacks = QString("%1-Attacks").arg(hostName);
        addVariable<double>(nameAttacks, &attacksHost[j], "Total number of attacks on host");
        QString nameAttacked = QString("%1-Attacked").arg(hostName);
        addVariable<double>(nameAttacked, &attackedHost[j], "Total number of attacked hosts");
    }
}

void Infection::updateCreatedVariables() {
    attacksVector.fill(0.);
    attackedVector.fill(0.);
    attacksHost.fill(0.);
    attackedHost.fill(0.);
    for (int i = 0; i < numPredators; ++i) {
        for (int j = 0; j < numPrey; ++j) {
            double hostDensity = *resources.at(j);
            double avgAttacksPerHost = (hostDensity > 0.) ? attacks.at(i,j)/hostDensity : 0.;
            propAttacked(i,j) = 1. - exp(-avgAttacksPerHost);
            attacked(i,j) = hostDensity*propAttacked(i,j);
            attacksVector[i] += attacks.at(i,j);
            attackedVector[i] += attacked.at(i,j);
            attacksHost[j] += attacks.at(i,j);
            attackedHost[j] += attacked.at(i,j);
        }
    }
}

void Infection::updateAttacksByPrey(int) {
    // nothing to do
}

} //namespace
