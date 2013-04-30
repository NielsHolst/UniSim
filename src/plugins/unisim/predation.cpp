/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QGenericMatrix>
#include <usbase/data_grid.h>
#include <usbase/utilities.h>
#include <usengine/simulation.h>
#include "predation.h"

namespace UniSim {


Predation::Predation(Identifier name, QObject *parent)
    : Model(name, parent), am(0)
{
    new Parameter<QString>("apparencyMatrix", &apparencyMatrixFileName, QString(""), this,
    "File with references to predator demands and apparency value for each predator x prey combiation");
}

Predation::~Predation() {
    delete am;
}

void Predation::amend() {
    QString fn = simulation()->inputFilePath(apparencyMatrixFileName);
    am = new DataGrid(fn, this);
    setPredators();
    setPrey();
    setApparencies();
    setAttacks();
    writeObjectTree(simulation());
}

void Predation::setPredators() {
    QStringList names = am->rowNames();
    if (names.isEmpty()) {
        QString msg = "No predators found in the rows of file '%1'";
        throw Exception(msg.arg(apparencyMatrixFileName));
    }
    createLinks(demands, names);
    extractNames(predatorNames, names);
    Q_ASSERT(demands.size() == predatorNames.size());
    numPredators = demands.size();
}

void Predation::setPrey() {
    QStringList names = am->columnNames().mid(1);
    if (names.isEmpty()) {
        QString msg = "No prey found in the columns of file '%1'";
        throw Exception(msg.arg(apparencyMatrixFileName));
    }
    createLinks(resources, names);
    extractNames(preyNames, names);
    Q_ASSERT(resources.size() == preyNames.size());
    numPrey = resources.size();
}

void Predation::createLinks(Links &links, QStringList refs) {
    links.clear();
    for (int i = 0; i < refs.size(); ++i) {
        QString name = refs[i];
        Variable<double> *variable = seekOne<Model*, Variable<double>* >(name);
        links << variable->valuePtr();
    }
}

void Predation::extractNames(QStringList &names, QStringList refs) {
    names.clear();
    for (int i = 0; i < refs.size(); ++i) {
        QStringList parts = splitParentChildExpression(refs[i]);
        names << parts[0];
    }
}

void Predation::setApparencies() {
    apparencies.resize(numPredators, numPrey);
    for (int i = 0; i < numPredators; ++i) {
        for (int j = 0; j < numPrey; ++j) {
            apparencies(i,j) = UniSim::stringToValue<double>(am->cell(i,j+1), this);
        }
    }
}

void Predation::setAttacks() {
    attacks.resize(numPredators, numPrey);
    for (int i = 0; i < numPredators; ++i) {
        for (int j = 0; j < numPrey; ++j) {
            QString name = QString("predation[%1-%2]")
                .arg(predatorNames[i])
                .arg(preyNames[j]);
            new Variable<double>(name, &attacks(i,j), this, "Number killed");
        }
    }
}

void Predation::reset() {
    update();
}

void Predation::update() {
    updateAttacksByPredator();
}

void Predation::updateAttacksByPredator() {
    for (int i = 0; i < numPredators; ++i) {
        double D = *demands.at(i);
        double sumAttacks = 0.;
        double sumWeightedRessources = 0.;

        for (int j = 0; j < numPrey; ++j) {
            double X = *resources.at(j);
            double a = apparencies.at(i,j);
            sumAttacks += attacks(i,j) = GBFuncResp(D, a*X);
            sumWeightedRessources += a*X;
        }

        double combinedAttacks = GBFuncResp(D, sumWeightedRessources);
        for (int j = 0; j < numPrey; ++j) {
            attacks(i,j) *= (sumAttacks == 0.) ? 0. : combinedAttacks/sumAttacks;
        }
    }
}

void Predation::updateAttacksByPrey() {
    QVector<double> proportionsWanted(numPrey);
    for (int j = 0; j < numPrey; ++j) {
        double X = *resources.at(j);
        for (int i = 0; i < numPredators; ++i) {
            proportionsWanted[i] = (X == 0.) ? 0 :attacks.at(i,j)/X;
        }
    }
}


} //namespace
