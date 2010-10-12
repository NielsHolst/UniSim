/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "output.h"
#include "output_data.h"
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
            xVar.append(variables[i]);
        else
            yVar.append(variables[i]);
    }

    data = seekChildren<OutputData*>("*");
    for (int i = 0; i < data.size(); ++i) {
        if (data[i]->axis() == OutputData::XAxis)
            xDat.append(data[i]);
        else
            yDat.append(data[i]);
    }
}

const OutputVariables& Output::xVariables() const {
    return xVar;
}

const OutputVariables& Output::yVariables() const {
    return yVar;
}

const OutputDatas& Output::xData() const {
    return xDat;
}

const OutputDatas& Output::yData() const {
    return yDat;
}
} //namespace

