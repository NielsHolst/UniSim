/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OUTPUT_VARIABLE_H
#define UNISIM_OUTPUT_VARIABLE_H

#include <QList>
#include <QVector>
#include <usbase/component.h>

namespace UniSim{	

class PullVariableBase;

class OutputVariable : public Component
{
	Q_OBJECT
public:
    OutputVariable(QString label, QString axis, PullVariableBase *variable, QObject *parent = 0);
	
    // standard methods
	void reset();
	void update();

    // special methods
    typedef enum {XAxis, YAxis} Axis;
    Axis axis() const;
    const QVector<double>* history() const;

private:
    // methods
    void setAxisFromString(QString axis);

    // data
    Axis _axis;
    QVector<double> _history;	//!< Series of collected values

    // links
    const Model *model;		//!< Pointer to the model holding the pull variable
    const PullVariableBase *pullVarPtr;
};

typedef QList<OutputVariable*> OutputVariables;

} //namespace


#endif
