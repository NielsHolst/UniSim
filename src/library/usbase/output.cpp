/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "integrator.h"
#include "output.h"
#include "parameter.h"
#include "trace_base.h"

namespace UniSim{
	
Output::Output(Identifier name, QObject *parent)
    : Component(name, parent), _hasSummary(false)
{
}

void Output::initialize() {
    _traces = seekChildren<TraceBase*>("*");
    _hasSummary = false;
    for (int i = 0; i < _traces.size(); ++i) {
        TraceBase *trace = _traces[i];
        if (trace->axis() == TraceBase::XAxis)
            _xTraces << trace;
        else
            _yTraces << trace;
        _hasSummary = _hasSummary || trace->summary() != TraceBase::None;
    }
    integrator = seekOne<Integrator*>("*");
}

const QList<TraceBase*>& Output::traces() const {
    return _traces;
}

const QList<TraceBase*>& Output::xTraces() const {
    return _xTraces;
}

const QList<TraceBase*>& Output::yTraces() const {
    return _yTraces;
}

int Output::runNumber() const {
    return integrator->pullVariable<int>("runNumber");
}

bool Output::hasSummary() const {
    return _hasSummary;
}

void Output::setYLabels() {
    if (!setYLabelsFromLabels() && yLabels.size() > 1)
        setYLabelsFromIds();
}

bool Output::setYLabelsFromLabels() {
    QList<NamedObject*> asNamedObjs;
    for (int i = 0; i < _yTraces.size(); ++i)
        asNamedObjs << _yTraces[i];

    bool areEqual;
    yLabels = getIds(asNamedObjs, &areEqual);
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
    QList<NamedObject*> parents;
    for (int i = 0; i < _yTraces.size(); ++i) {
        NamedObject *parent = _yTraces[i]->traceParent();
        Q_ASSERT(parent);
        parents << parent;
    }

    bool areEqual(true), checkAgain(true);
    while (areEqual && checkAgain) {
        yLabels = getIds(parents, &areEqual);
        if (areEqual) {
            for (int i = 0; checkAgain && (i < parents.size()); ++i) {
                parents[i] = dynamic_cast<NamedObject*>(parents[i]->parent());
                checkAgain = parents[i];
            }
        }
    }
}

} //namespace

