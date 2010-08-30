/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "output.h"
#include "output_variable.h"
#include "parameter.h"

namespace UniSim{
	
Output::Output(Identifier name, QObject *parent)
    : Component(name, parent)
{
}

void Output::initialize() {
    variables = seekChildren<OutputVariable*>("*");
    for (int i = 0; i < variables.size(); ++i) {
        if (variables[i]->axis() == OutputVariable::XAxis)
            x.append(variables[i]);
        else
            y.append(variables[i]);
    }
}

const OutputVariables& Output::xVariables() const {
    return x;
}

const OutputVariables& Output::yVariables() const {
    return y;
}
} //namespace

