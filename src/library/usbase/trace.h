/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_TRACE_H
#define UNISIM_TRACE_H

#include <QList>
#include <QVector>
#include "attributes.h"
#include "component.h"

namespace UniSim{	

class Model;
class OutputBase;
class VariableBase;

class Trace : public Component, public Attributes
{
public:
    Trace(QString name, VariableBase *variable, QObject *parent = 0);
	
    // standard methods
    void amend();
    void reset();
    void update();
    void cleanup();
    void debrief();

    // special methods
    enum Summary {None, Max, Min, Average, Sum, Final, XAtThreshold, XAtMax, XAtMin};
    Summary summary() const;
    enum Type {Line, Symbols, Both} ;
    Type type() const;
    bool hasWildCard() const;

    OutputBase* traceParent();
    Model* variableParent();
    QVector<double>* history();

private:
    // parameters
    int sample;
    // methods
    void setScaling();
    void setSummary();
    void setType();
    void setThreshold(QString summaryCode);
    bool isSummary() const;

    void resetTime();
    void resetSummary();
    void updateSummary();

    double currentValue();

    // data
    VariableBase *variable;
    Summary _summary;
    Type _type;
    struct {
        int n;
        double value, sum, prevValue, minValue, maxValue, threshold, x;
        bool pastThreshold, hasPrevValue;
    } s;

    double multiplier, divisor;

    QVector<double> _history;	//!< Series of collected values
    bool historyCleared;
    int sampleCount;
    double sampleSum;
    bool hasCalendarParent;

    // links
    const OutputBase *output;   //!< Pointer to parent or null

};


} //namespace


#endif
