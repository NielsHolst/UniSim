/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include <usbase/data_grid.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include "functional_response_multiway.h"

using namespace std;

namespace UniSim {


FunctionalResponseMultiway::FunctionalResponseMultiway(Identifier name, QObject *parent)
    : Model(name, parent), am(0)
{
    addParameter<QString>("apparancyMatrix", &apparancyMatrixFileName, QString(""),
    "File with references to predator demands and apparency value for each predator x prey combination");
}

void FunctionalResponseMultiway::amend() {
    QString fn = simulation()->inputFilePath(apparancyMatrixFileName);
    am = new DataGrid(fn, this);
    setPredators();
    setPrey();
    setApparencies();
    attacks.resize(numPredators, numPrey);
    createVariables();
}

void FunctionalResponseMultiway::setPredators() {
    QStringList names = am->rowNames();
    if (names.isEmpty()) {
        QString msg = "No predators found in the rows of file '%1'";
        throw Exception(msg.arg(apparancyMatrixFileName));
    }
    createLinks(demands, names);
    extractNames(predatorNames, names);
    Q_ASSERT(demands.size() == predatorNames.size());
    numPredators = demands.size();
}

void FunctionalResponseMultiway::setPrey() {
    QStringList names = am->columnNames().mid(1);
    if (names.isEmpty()) {
        QString msg = "No prey found in the columns of file '%1'";
        throw Exception(msg.arg(apparancyMatrixFileName));
    }
    createLinks(resources, names);
    extractNames(preyNames, names);
    Q_ASSERT(resources.size() == preyNames.size());
    numPrey = resources.size();
}

void FunctionalResponseMultiway::createLinks(Links &links, QStringList refs) {
    links.resize(refs.size());
    for (int i = 0; i < refs.size(); ++i) {
        QString name = refs[i];
        Variable<double> *variable = seekOne<Model*, Variable<double>* >(name);
        links[i] = variable->valuePtr();
    }
}

void FunctionalResponseMultiway::extractNames(QStringList &names, QStringList refs) {
    names.clear();
    for (int i = 0; i < refs.size(); ++i) {
        QStringList parts = splitParentChildExpression(refs[i], this);
        names << parts[0];
    }
}

void FunctionalResponseMultiway::setApparencies() {
    apparencies.resize(numPredators, numPrey);
    for (int i = 0; i < numPredators; ++i) {
        for (int j = 0; j < numPrey; ++j) {
            apparencies(i,j) = UniSim::stringToValue<double>(am->cell(i,j+1), this);
        }
    }
}

void FunctionalResponseMultiway::reset() {
    attacks.fill(0.);
}

void FunctionalResponseMultiway::update() {
    updateAttacksByPredator();
    updateAttacksByPrey();
    updateCreatedVariables();
}

void FunctionalResponseMultiway::updateAttacksByPredator() {
    for (int i = 0; i < numPredators; ++i)
        updateAttacksByPredator(i);
}

void FunctionalResponseMultiway::updateAttacksByPredator(int ixPredator) {
    double D = *demands.at(ixPredator);
    double sumAttacks = 0.;
    double sumWeightedRessources = 0.;

    for (int j = 0; j < numPrey; ++j) {
        double X = *resources.at(j);
        double a = apparencies.at(ixPredator,j);
        sumAttacks += attacks(ixPredator,j) = GBFuncResp(D, a*X);
        sumWeightedRessources += a*X;
    }

    double combinedAttacks = GBFuncResp(D, sumWeightedRessources);
    for (int j = 0; j < numPrey; ++j) {
        attacks(ixPredator,j) *= (sumAttacks == 0.) ? 0. : combinedAttacks/sumAttacks;
    }
}

void FunctionalResponseMultiway::updateAttacksByPrey() {
    for (int j = 0; j < numPrey; ++j)
        updateAttacksByPrey(j);
}


} //namespace
