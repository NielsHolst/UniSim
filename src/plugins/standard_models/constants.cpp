/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
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
    for (QMap<Identifier,double*>::iterator va=variables.begin(); va!=variables.end(); ++va )
        delete va.value();
    variables.clear();
}

void Constants::initialize() {
    setParameter("variables", &strVariables, QString(),
                 "List of name value pairs. A pull-push variables will be created for each pair"
                 "associating the name with the given value, e.g, ((TAvg 20.)(rainfall 0.))");

    QString errorContext = "Variables list in " + objectName();
    QStringList items = decodeSimpleList(strVariables, errorContext);
    for (int i = 0; i < items.size(); ++i) {
        QString errorContext2 =
                errorContext +": Items in strings should be (name number) pairs. "
                "Got: '" + items[i] + "'" ;
        QStringList pair = decodeSimpleList(items[i], errorContext2);
        bool ok;
        double number = pair[1].toDouble(&ok);
        if (pair.size() != 2 || !ok)
            throw Exception(errorContext2);

        double *valuePtr = new double;
        *valuePtr = number;
        Identifier id = Identifier(pair[0]);
        variables[id] = valuePtr;
        new PushVariable(id, valuePtr, this, "Constant");
        new PullVariable(id, valuePtr, this, "Constant");
    }
}

} //namespace

