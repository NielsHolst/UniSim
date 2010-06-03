/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MODEL_H
#define UNISIM_MODEL_H

#include "component.h"
#include "identifier.h"
#include "parameters.h"

namespace UniSim{
	
class Model : public UniSim::Component, public UniSim::Parameters
{
	Q_OBJECT
public:
    Model(Identifier name, QObject *parent=0);

    void pushVariable(Identifier name, double value);
    double pullVariable(Identifier name);
    const double* pullVariablePtr(Identifier name);
};

typedef QList<Model*> Models;

} //namespace

#endif
