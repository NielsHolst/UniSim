/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_H
#define UNISIM_OUTPUT_H

#include <QList>
#include <usbase/component.h>
#include "attributes.h"

namespace UniSim{

class DataGrid;
class Integrator;
class Trace;

class OutputBase : public Component, public Attributes
{
public:
    OutputBase(Identifier name, QObject *parent=0);
    virtual ~OutputBase(){}
    // standard methods
    void amend();
    // special methods
    struct TraceRecord {
        Trace *trace;
        QString label;
        //int index;
    };

    QList<TraceRecord>& traceRecords();
    bool hasSummary() const;
    virtual int numXTraces() const = 0;
protected:
    // parameters
    int sample;
    // methods
    int runNumber() const;

private:
    // data
    bool _hasSummary;
    // links
    QList<TraceRecord> _traceRecords;
    Integrator *integrator;
    // methods
    void setYLabels();
};

typedef QList<OutputBase*> Outputs;

} //namespace

#endif

