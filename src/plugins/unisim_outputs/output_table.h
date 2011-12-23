/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_TABLE
#define UNISIM_OUTPUT_TABLE

#include <QFile>
#include <QList>
#include <usbase/output.h>
#include <usbase/trace_base.h>

namespace UniSim{

class OutputTable : public Output
{
	Q_OBJECT
public:
    OutputTable(Identifier name, QObject *parent=0);
    // standard methods
    void cleanup();
    void debrief();

private:
    // parameters
    QString fileName;

    // housekeeping
    QFile file;

    // methods
    void openFile();
    void closeFile();
    QString ammendedFileName(QString fileName, int number);
    int traceSize() const;
    void writeLabels();
    void writeXLabels();
    void writeYLabels();
    void writeTraces();
    void writeTraces(const QList<TraceBase*> &results, int dataIx);
    void writeCR();
    void writeTab();
};

} //namespace


#endif
