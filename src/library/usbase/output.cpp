/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "integrator.h"
#include "output.h"
#include "output_result.h"
#include "output_variable.h"
#include "parameter.h"

namespace UniSim{
	
Output::Output(Identifier name, QObject *parent)
    : Component(name, parent), _isSummary(false)
{
}

void Output::initialize() {
    results = seekChildren<OutputResult*>("*");
    for (int i = 0; i < results.size(); ++i) {
        if (results[i]->axis() == OutputResult::XAxis)
            xRes.append(results[i]);
        else
            yRes.append(results[i]);
    }

    integrator = seekOne<Integrator*>("*");
}

const OutputResults& Output::xResults() const {
    return xRes;
}

const OutputResults& Output::yResults() const {
    return yRes;
}

int Output::runNumber() const {
    return integrator->pullVariable<int>("runNumber");
}

bool Output::isSummary() const {
    return _isSummary;
}

void Output::setIsSummary(bool value) {
    _isSummary = value;
}


void Output::setYLabels() {
    if (!setYLabelsFromLabels() && yLabels.size() > 1)
        setYLabelsFromIds();
}

bool Output::setYLabelsFromLabels() {
    QList<NamedObject*> namedObjs;
    for (int i = 0; i < yResults().size(); ++i)
        namedObjs.append(yResults()[i]);

    bool areEqual;
    yLabels = getIds(namedObjs, &areEqual);
    return !areEqual;
}

QList<Identifier> Output::getIds(QList<NamedObject*> &objects, bool *areEqual) const{
    QList<Identifier> ids;
    *areEqual = true;
    Identifier prevId;
    for (int i = 0; i < objects.size(); ++i) {
        Identifier id = objects[i]->id();
        ids.append(id);
        *areEqual = *areEqual && (i==0 || id==prevId);
        prevId = id;
    }
    return ids;
}

void Output::setYLabelsFromIds() {
    QList<NamedObject*> ancestors;
    for (int i = 0; i < yResults().size(); ++i) {
        OutputVariable *var = dynamic_cast<OutputVariable*>(yResults()[i]);
        Q_ASSERT(var);
        NamedObject *ancestor = dynamic_cast<NamedObject*>(var->pullVariable()->parent());
        Q_ASSERT(ancestor);
        ancestors.append(ancestor);
    }

    bool areEqual(true), checkAgain(true);
    while (areEqual && checkAgain) {
        yLabels = getIds(ancestors, &areEqual);
        if (areEqual) {
            for (int i = 0; checkAgain && (i < ancestors.size()); ++i) {
                ancestors[i] = dynamic_cast<NamedObject*>(ancestors[i]->parent());
                checkAgain = ancestors[i];
            }
        }
    }
}

} //namespace

