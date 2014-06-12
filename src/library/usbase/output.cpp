/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include "trace.h"

namespace UniSim{
	
OutputBase::OutputBase(Identifier name, QObject *parent)
    : Component(name, parent), _hasSummary(false)
{
}


void OutputBase::amend() {
    QList<Trace*> traces = seekChildren<Trace*>("*");
    _hasSummary = false;
    for (int i = 0; i < traces.size(); ++i) {
        TraceRecord rec;
        rec.trace = traces[i];
        rec.label = rec.trace->id().label();
        _traceRecords << rec;
        _hasSummary = (_hasSummary || rec.trace->summary() != Trace::None);
    }
    setYLabels();
    integrator = seekOne<Integrator*>("*");
}

QList<OutputBase::TraceRecord>& OutputBase::traceRecords() {
    return _traceRecords;
}

int OutputBase::runNumber() const {
    return integrator->pullValue<int>("runNumber");
}

bool OutputBase::hasSummary() const {
    return _hasSummary;
}

void OutputBase::setYLabels() {
    QStringList sl;
    int numX = numXTraces();
    for (int i = numX; i < _traceRecords.size(); ++i) {
        TraceRecord &rec( _traceRecords[i] );
        NamedObject *parent = rec.trace->variableParent();
        QString name;
        if (parent)
            name = parent->fullLabel() + "/";
        name += rec.label;
        sl << name;
    }
    NameList nl(sl);
    QStringList yLabels = nl.simplified();
    for (int i = numX; i < _traceRecords.size(); ++i) {
        TraceRecord &rec( _traceRecords[i] );
        rec.label = yLabels[i-numX];
    }
}

} //namespace

