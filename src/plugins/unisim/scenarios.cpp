/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/utilities.h>          
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include <usbase/ref.h>
#include <usengine/simulation.h>
#include "scenarios.h"

namespace UniSim {


Scenarios::Scenarios(Identifier name, QObject *parent)
    : Model(name, parent)
{
    addParameter<QString>(Name(fileName), QString("scenarios.txt"),"Column-oriented text file with scenarios, line by line");
    addVariable<int>(Name(iteration), "Number of current iteration");
    addVariable<int>(Name(numIterations), "Number of current iteration");
    addVariable<bool>(Name(value), "Is current iteration within the set number of iterations?");
}

void Scenarios::amend() {
    scenarios = new DataGrid(simulation()->inputFilePath(fileName), this);
    parseColumnNames();
    iteration = 1;
    numIterations = scenarios->rowNumber();
    value = true;
}

void Scenarios::reset() {
    QVector<QString> newLine = scenarios->row<QString>(iteration-1);
    for (int i = 0; i < refs.size(); ++i) {
        if (refs[i] == 0) {
            line[i] = newLine[i];
        }
        else try {
            refs[i]->setValueFromString(newLine[i]);
        }
        catch (Exception &ex) {
            QString msg = "Could not set parameter value from Scenarios model. "
                    "Column number: %1. Row (not counting row with column labels: %2. Value= '%3'.\n%4";
            throw Exception(msg.arg(i).arg(iteration).arg(newLine[i]).arg(ex.message()), this);
        }
    }
}

void Scenarios::cleanup() {
    ++iteration;
    value = (iteration <= numIterations);
}

void Scenarios::parseColumnNames() {
    QStringList names = scenarios->columnNames();
    line.resize(names.size());
    refs.resize(names.size());
    for (int i = 0; i < names.size(); ++i) {
        QString name = names[i];
        if (name.contains("[")) {
            try {
                refs[i] = seekOne<NamedObject*, ParameterBase*>(name);
            }
            catch (Exception &ex) {
                QString msg = "Scenarios model could not find parameter named '%1' (from label of column %2):\n%3";
                throw Exception(msg.arg(name).arg(i).arg(ex.message()), this);
            }
        }
        else {
            refs[i] = 0;
            addParameter<QString>(name, &line[i], QString(), "Column parameter");
        }
    }
}


} //namespace
