/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/identifier.h>
#include <usbase/pull_variable.h>
#include <usbase/push_variable.h>
#include <usbase/utilities.h>
#include "constants.h"

namespace UniSim{

Constants::Constants(Identifier name, QObject *parent)
	: Model(name, parent)
{
}

Constants::~Constants() {
    for (int i = 0; i < valuePtrs.size(); ++i)
        delete valuePtrs[i];
    valuePtrs.clear();
}

void Constants::initialize() {
    Identifiers params = allParameters();
    valuePtrs.clear();
    for (int i = 0; i < params.size(); ++i) {
        QString stringValue = parameterAsString(params[i]);
        bool ok;
        double value = stringValue.toDouble(&ok);
        if (!ok)
            throw Exception("Parameter value for Constants model is not a number: " +
                            params[i].label() + " = " + stringValue);
        addPullVariable<double>(params[i], value);
    }
}

void Constants::addPullVariable<double>(Identifier id, double value) {
    double *valuePtr = new double(value);
    valuePtrs.append(valuePtr);
    new PullVariable<double>(id, valuePtr, this, "Constant");
}

} //namespace
