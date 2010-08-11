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
#include <usbase/parameters.h>

namespace UniSim{	

class PullVariableBase;

class OutputVariable : public Component, public Parameters
{
	Q_OBJECT
public:
    OutputVariable(Identifier name, QObject *parent=0);
	
    // standard methods
	void initialize();
	void reset();
	void update();

    // special methods
    struct Raw{
        QString label, axis, modelName, stateNameInModel;
    };
    static void appendVariable(Raw raw, QObject *parent);
    void extendLabel();
    void standardizeLabel();

    typedef enum {XAxis, YAxis} Axis;
    Axis axis() const;
    QString label() const;
    QString longName() const;
    const QVector<double>* data() const;

private:
    // parameters
    QString _label, modelName, stateNameInModel;

    // links
    const Model *model;		//!< Pointer to the model holding the pull variable
    const PullVariableBase *pullVarPtr;

    // state
    Axis _axis;
    QVector<double> _data;	//!< Series of collected values

    // methods
    void fromRaw(Raw raw);
};

typedef QList<OutputVariable*> OutputVariables;

} //namespace


#endif
