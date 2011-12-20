/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include "model.h"
#include "output_parameter.h"
#include "parameter_base.h"
#include "utilities.h"

namespace UniSim{
	
OutputParameter::OutputParameter(const QMap<QString,QString> &attributes,  ParameterBase *parameter, QObject *parent)
    : OutputResult(attributes, parent), parameterPtr(parameter)
{
    Q_ASSERT(parameterPtr);
}

void OutputParameter::update() {
    Q_ASSERT(parameterPtr);
    double toAppend = parameterPtr->toVariant().value<double>();
    _history.append(toAppend);
}


} //namespace

