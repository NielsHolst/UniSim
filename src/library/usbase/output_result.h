/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_RESULT_H
#define UNISIM_OUTPUT_RESULT_H

#include <QList>
#include <QVector>
#include "component.h"

namespace UniSim{	

class Output;

class OutputResult : public Component
{
	Q_OBJECT
public:
    OutputResult(QString label, QString axis, QObject *parent = 0);
	
    // standard methods
    void initialize();
	void reset();
    void debrief();

    // special methods
    enum Axis {XAxis, YAxis};
    enum Summary {None, Max, Min, Average, Sum, Final, XAtThreshold, XAtMax, XAtMin};

    Axis axis() const;
    Summary summary() const;

    const QVector<double>* history() const;
    bool isOutputSummary() const;

protected:
    // methods
    void setAxisFromString(QString axis);
    void setSummaryFromString(QString axis);

    // data
    QString axisString;
    Axis _axis;
    Summary _summary;
    QVector<double> _history;	//!< Series of collected values
    bool historyCleared;

    // links
    const Output *output;   //!< Pointer to parent or null

};

typedef QList<OutputResult*> OutputResults;

} //namespace


#endif
