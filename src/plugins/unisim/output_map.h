/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_MAP
#define UNISIM_OUTPUT_MAP

#include <QPair>
#include <QVector>
#include "output_table_base.h"

namespace UniSim{

class Trace;

class OutputMap : public OutputTableBase
{
	//Q_OBJECT
public:
    OutputMap(Identifier name, QObject *parent=0);
    // standard methods
    void initialize();
    void cleanup();
private:
    // parameters
    QString rangeAsString;
    QPair<double,double> range;
    int steps;
    bool doLog10;

    // types
    struct PatchInfo {
        int x, y, size;
    };


    // methods
    void setRange();
    void setPatches();
    int findValue(Trace *trace, QString name);
    void writeTraces();
    void writeTime(int time, bool atEnd);
    QString buildSet(int time, QString subId, bool atEnd = false);
    void writeRect(PatchInfo pi, double value);
    void writeRectGroupEnd(int time);
    QString colour(double value);

    // data
    QVector<Trace*> traces;
    QVector<PatchInfo> patches;
};

} //namespace


#endif
