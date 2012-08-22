/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QSet>
#include "data_grid.h"
#include "integrator.h"
#include "name_list.h"
#include "output.h"
#include "parameter.h"
#include "trace_base.h"

namespace UniSim{
	
Output::Output(Identifier name, QObject *parent)
    : Component(name, parent), _hasSummary(false)
{
}

void Output::amend() {
    _traces = seekChildren<TraceBase*>("*");
    _hasSummary = false;
    int yIndex = 0;
    for (int i = 0; i < _traces.size(); ++i) {
        TraceRecord rec;
        rec.trace = _traces[i];
        rec.label = rec.trace->id().label();
        rec.index = 0;
        if (rec.trace->axis() == TraceBase::XAxis)
            _xTraces << rec;
        else {
            rec.index = yIndex++;
            _yTraces << rec;
        }
        _hasSummary = _hasSummary || rec.trace->summary() != TraceBase::None;
    }
    setYLabels();
    integrator = seekOne<Integrator*>("*");
}

const QList<TraceBase*>& Output::traces() const {
    return _traces;
}

QList<Output::TraceRecord>& Output::xTraces() {
    return _xTraces;
}

QList<Output::TraceRecord>& Output::yTraces() {
    return _yTraces;
}

int Output::runNumber() const {
    return integrator->pullValue<int>("runNumber");
}

bool Output::hasSummary() const {
    return _hasSummary;
}

void Output::setYLabels() {
    QStringList sl;
    for (int i = 0; i < _yTraces.size(); ++i) {
        TraceRecord &rec( _yTraces[i] );
        NamedObject *parent = rec.trace->variableParent();
        QString name;
        if (parent)
            name = parent->fullLabel() + "/";
        name += rec.label;
        sl << name;
    }
    NameList nl(sl);
    QStringList yLabels = nl.simplified();
    for (int i = 0; i < _yTraces.size(); ++i) {
        TraceRecord &rec( _yTraces[i] );
        rec.label = yLabels[i];
    }
}

} //namespace

