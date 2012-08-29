/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_TABLE
#define UNISIM_OUTPUT_TABLE

#include <QFile>
#include <QList>
#include <usbase/file_location_info.h>
#include "output_table_base.h"

namespace UniSim{

class OutputTable : public OutputTableBase
{
	//Q_OBJECT
public:
    OutputTable(Identifier name, QObject *parent=0);
    // standard methods
    void cleanup();
    void debrief();
private:
    // methods
    void writeLabels();
    void writeTraces();
    void writeTraces(const QList<TraceRecord> &traces, int dataIx);
};

} //namespace


#endif
