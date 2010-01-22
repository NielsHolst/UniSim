/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QRegExp>
#include "model.h"
#include "output_variable.h"
#include "utilities.h"

namespace UniSim{
	
OutputVariable::OutputVariable(Identifier name, QObject *parent)
    : Component(name, parent)
{
}

void OutputVariable::initialize() {
}

void OutputVariable::reset() {
    _data.clear();
}

void OutputVariable::update() {
    _data.append(*statePtr);
}

OutputVariable::Axis OutputVariable::axis() const {
    return _axis;
}

QString OutputVariable::label() const {
    return _label;
}

QString OutputVariable::longName() const {
    return _label + ":" + modelName + "[" + stateNameInModel + "]";
}

const QVector<double>* OutputVariable::data() const {
    return &_data;
}

void OutputVariable::appendVariable(OutputVariable::Raw raw, QObject *parent) {
    Models models = find<Model*>(raw.modelName);

    int numVariables = 0;
    for (Models::iterator mo = models.begin(); mo != models.end(); ++mo) {
        if (raw.stateNameInModel == "*") {
            for (Model::States::const_iterator st = (*mo)->states().begin(); st != (*mo)->states().end(); ++st) {
                Identifier stateNameInModel = st.key();
                OutputVariable *v = new OutputVariable(stateNameInModel, parent);
                v->fromRaw(raw);
                v->model = *mo;
                v->statePtr = st.value();
                ++numVariables;
            }
        }
        else {
            const double *statePtr = (*mo)->statePtr(raw.stateNameInModel);
            if (statePtr) {
                OutputVariable *v = new OutputVariable(raw.stateNameInModel, parent);
                v->fromRaw(raw);
                v->model = *mo;
                v->statePtr = statePtr;
                ++numVariables;
            }
        }
    }
    if (numVariables == 0)
        throw Exception("Could not find variable: " + raw.label);

}

void OutputVariable::extendLabel() {
    _label = modelName + "_" + _label;
}

void OutputVariable::standardizeLabel() {
    //QRegExp unwanted("[[^a-z][^A-Z][^0-9]]");
    QRegExp unwanted("[ /]");
    _label.replace(unwanted, "_");
}

void OutputVariable::fromRaw(Raw raw) {
    raw.axis = raw.axis.toLower();
    Q_ASSERT(raw.axis=="x" || raw.axis=="y");
    _label = raw.label;
    modelName = raw.modelName;
    stateNameInModel = raw.stateNameInModel;
    _axis = raw.axis == "x" ? XAxis : YAxis;
}

} //namespace
