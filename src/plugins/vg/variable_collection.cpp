/* Copyright (C) 2013 by Oliver Koerner, AgroTech [oko@agrotech.dk] and
** Niels Holst, Aarhus University [niels.holst@agrsci.dk].
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/exception.h>
#include <usbase/model.h>
#include "publish.h"
#include "variable_collection.h"

using namespace UniSim;

namespace vg {
	
VariableCollection::VariableCollection() {
}

void VariableCollection::set(UniSim::NamedObject *source, QString nameExpression, bool assertNotEmpty) {
    variables = source->seekMany<Model*, Variable<double>*>(nameExpression);
    if (assertNotEmpty && variables.empty()) {
        QString msg = "Found no variables matching '%1'";
        throw Exception(msg.arg(nameExpression), source);
    }
}

double VariableCollection::sum() const {
    double result = 0.;
    for (int i = 0; i < variables.size(); ++i) {
        result += *( variables.at(i)->valuePtr() );
    }
    return result;
}

double VariableCollection::average() const {
    return (size() == 0) ? 0. : sum()/size();
}

double VariableCollection::product() const {
    double result = 1.;
    for (int i = 0; i < variables.size(); ++i) {
        result *= *( variables.at(i)->valuePtr() );
    }
    return result;
}

int VariableCollection::size() const {
    return variables.size();
}

} //namespace

