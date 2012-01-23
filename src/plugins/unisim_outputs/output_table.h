/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_TABLE
#define UNISIM_OUTPUT_TABLE

#include <QFile>
#include <QList>
#include <usbase/file_location_info.h>
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
    // special methods
    QStringList outputFilePaths();

protected:
    FileLocationInfo::FileType fileType;
private:
    // parameters
    QString fileName;

    // housekeeping
    QFile file;
    QStringList _outputFilePaths;

    // methods
    void openFile();
    void closeFile();
    QString amendedFileName(QString fileName, int number);
    int traceSize() const;
    void writeLabels();
    void writeLabels(const QList<TraceRecord> &traces);
    void writeTraces();
    void writeTraces(const QList<TraceRecord> &traces, int dataIx);
    void writeCR();
    void writeTab();
};

} //namespace


#endif
