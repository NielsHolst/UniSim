/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_H
#define UNISIM_OUTPUT_H

#include <QList>
#include <usbase/component.h>

namespace UniSim{

class DataGrid;
class Integrator;
class Trace;

class Output : public Component
{
	//Q_OBJECT
public:
    Output(Identifier name, QObject *parent=0);
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
protected:
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

typedef QList<Output*> Outputs;

} //namespace

#endif

