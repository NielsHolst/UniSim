/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_CROSSTAB
#define UNISIM_OUTPUT_CROSSTAB

#include <QSet>
#include <usbase/model.h>
#include "output_table_base.h"

namespace UniSim{

class Model;
class Trace;

class OutputCrosstab : public OutputTableBase
{
	Q_OBJECT
public:
    OutputCrosstab(Identifier name, QObject *parent=0);
    // standard methods
    void amend();
    void cleanup();
    void debrief();
private:
    QString rowClass, columnClass;
    typedef QPair<QString, QString> TraceKey;
    QMap< TraceKey, Trace*> traceMatrix;
    QStringList rowNames, columnNames;

    void checkTraces();
    void checkAttribute(Trace *trace, QString attr);
    void checkAttribute(Trace *trace, QString attr, QString value);
    Model* seekParent(Trace *trace, QString parentClass);
    void writeFiles();
    void writeColumnLabels();
    void writeHistoryToFile(int ixHistory);
};

} //namespace


#endif
