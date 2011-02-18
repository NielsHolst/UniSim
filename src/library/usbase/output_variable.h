/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_VARIABLE_H
#define UNISIM_OUTPUT_VARIABLE_H

#include <QList>
#include <QVector>
#include "output_result.h"

namespace UniSim{	

class PullVariableBase;

class OutputVariable : public OutputResult
{
	Q_OBJECT
public:
    OutputVariable(QString label, QString axis, QString summary, PullVariableBase *variable, QObject *parent = 0);
	
    // standard methods
    void initialize();
    void reset();
    void update();
    void cleanup();

    // special methods
    const PullVariableBase *pullVariable();

private:
    // methods
    void setThreshold(QString summary);
    QPair<QString, double> decodeSummary(QString summary);
    void resetSummary();
    void updateSummary(double value);

    // data
    QString summaryString;
    struct {
        int n;
        double value, sum, prevValue, minValue, maxValue, threshold, x;
        bool passedThreshold, hasPrevValue;
    } s;

    // links
    const PullVariableBase *pullVarPtr;
};

typedef QList<OutputVariable*> OutputVariables;

} //namespace


#endif
