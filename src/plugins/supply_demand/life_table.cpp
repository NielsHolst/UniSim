/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <usbase/exception.h>
#include <usbase/parameter.h>
#include <usbase/variable.h>
#include <usbase/test_num.h>
#include <usbase/utilities.h>
#include "holometabola.h"
#include "life_table.h"
#include "life_stage.h"

using namespace UniSim;


namespace SupplyDemand {

LifeTable::LifeTable(UniSim::Identifier name, QObject *parent)
	: Model(name, parent)
{
    setRecursionPolicy(Update,ChildrenNot);

    new Parameter<QString>("preceding", &strPreceding, QString("()"), this,
    "Corresponding @F LifeTable in preceding @F {LifeStage}. Defined as a list of strings to allow convergence of more than one life stage.");
    new Variable<double>("individualMass", &individualMass, this, "Average mass of one individual (g)");
}

void LifeTable::initialize() {
    Holometabola *holometabola = seekOneAscendant<Holometabola*>("*");
    QList<QString> listPreceding = decodeList<QString>(strPreceding, this);
    for (int i= 0; i < listPreceding.size(); ++i) {
        LifeStage* preceding = holometabola->seekOneDescendant<LifeStage*>(listPreceding[i]);
        precedingMass.append( preceding->seekOneDescendant<Model*>("lifetable/mass") );
        precedingNumber.append( preceding->seekOneDescendant<Model*>("lifetable/number") );
    }

    massModel = seekOneChild<Model*>("mass");
    numberModel = seekOneChild<Model*>("number");
    mass = massModel->pullValuePtr<double>("value");
    number = numberModel->pullValuePtr<double>("value");
    seekSplitModel();
}

void LifeTable::seekSplitModel() {
    doSplit = false;
    LifeStage *stage = seekNearestAscendant<LifeStage*>("*");
    Model *splitModel = stage->peekOneSibling<Model*>("split");
    if (splitModel) {
        doSplit = true;
        split[0] = splitModel->pullValuePtr<double>("first");
        split[1] = splitModel->pullValuePtr<double>("second");
        myPosition = stage->seekSiblingPosition<LifeStage*>("*");
        if (myPosition > 1)
            throw Exception("There can be at most two sibling LifeStage models together with a split model", this);
    }
}

void LifeTable::reset() {
    individualMass = 0.;
}

void LifeTable::update() {
    double massInflow = massModel->pullValue<double>("inflow") + inflow(precedingMass);
    massModel->pushValue<double>("inflow", massInflow);
    massModel->deepUpdate();

    double numberInflow = numberModel->pullValue<double>("inflow") + inflow(precedingNumber);
    numberModel->pushValue<double>("inflow", numberInflow);
    numberModel->deepUpdate();

    individualMass = TestNum::eqZero(*number) ? 0 : divBounded(*mass,*number,1e3);
}

double LifeTable::inflow(Models preceding) {
    double sum = 0.;
    for (int i = 0; i < preceding.size(); ++i) {
        sum += preceding[i]->pullValue<double>("outflow");
    }
    if (doSplit)
        sum *= *split[myPosition];
    Q_ASSERT(sum >= 0);
    return sum;
}

} //namespace

