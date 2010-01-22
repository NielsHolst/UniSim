/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MODEL_H
#define UNISIM_MODEL_H

#include <QMap>
#include "component.h"
#include "identifier.h"
#include "parameters.h"

namespace UniSim{
	
class Model : public UniSim::Component, public UniSim::Parameters
{
	Q_OBJECT
public:
    typedef QMap<Identifier, const double*> States;
    typedef QMap<Identifier, double> Inputs;

    Model(Identifier name, QObject *parent=0);

    void setState(Identifier name, double *var);
    double state(Identifier name) const;
    const double* statePtr(Identifier name) const;
    const States& states() const;

    void setInput(Identifier name, double input);
	
protected:
    double input(Identifier name) const;

private:
	States _states;
	Inputs _inputs;
	
	QString allStates() const;
};

typedef QList<Model*> Models;

} //namespace

#endif
