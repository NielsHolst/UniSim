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
class Output;
class VariableBase;

class Trace : public Component, public Attributes
{
	//Q_OBJECT
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

    Output* traceParent();
    Model* variableParent();
    double currentValue();
    QVector<double>* history();

private:
    // methods
    void setSummary();
    void setType();
    void setThreshold(QString summaryCode);
    bool isSummary() const;

    void resetSummary();
    void updateSummary();

    // data
    VariableBase *variable;
    Summary _summary;
    Type _type;
    struct {
        int n;
        double value, sum, prevValue, minValue, maxValue, threshold, x;
        bool pastThreshold, hasPrevValue;
    } s;

    QVector<double> _history;	//!< Series of collected values
    bool historyCleared;

    // links
    const Output *output;   //!< Pointer to parent or null

};


} //namespace


#endif
