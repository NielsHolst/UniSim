/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_MODEL_H
#define UNISIM_MODEL_H

#include <QVariant>
#include "component.h"
#include "exception.h"
#include "identifier.h"
#include "utilities.h"
#include "parameter.h"
#include "parameter_vector.h"
#include "variable.h"
#include "variable_vector.h"

#include "name.h"

namespace UniSim{

class Model : public UniSim::Component
{
public:
    Model(Identifier name, QObject *parent=0);

    template <class T> T pullValue(Identifier name);
    template <class T> const T* pullValuePtr(Identifier name);
    template <class T> const T* peekValuePtr(Identifier name);
    template <class T> void pushValue(Identifier name, T value);

    template <class T> const QVector<T>& pullValueVector(Identifier name);
    template <class T> const QVector<T>* pullValueVectorPtr(Identifier name);
    template <class T> void pushValueVector(Identifier name, const QVector<T> &value);

    Identifier classId();
    QString peekKeyValue(Identifier key);

    bool hide() const;
    void setHide(bool value);

    Model* prevInstance() { return prev; }
    Model* nextInstance() { return next; }
    bool hasInstances();
    void setInstances(Model *prev, Model *next);
private:
    bool _hide;
    Model *prev, *next;
};

typedef QList<Model*> Models;

template <class T>
T Model::pullValue(Identifier name)
{
    return seekOneChild<Variable<T>*>(name.key())->value();
}

template <class T>
const T* Model::pullValuePtr(Identifier name)
{
    return seekOneChild<Variable<T>*>(name.key())->valuePtr();
}

template <class T>
const T* Model::peekValuePtr(Identifier name)
{
    Variable<T> *var = peekOneChild<Variable<T>*>(name.key());
    return var ? var->valuePtr() : 0;
}

template <class T>
void Model::pushValue(Identifier name, T value)
{
    seekOneChild<Parameter<T>*>(name.key())->setValue(value);
}

template <class T>
const QVector<T>& Model::pullValueVector(Identifier name) {
    return seekOneChild<VariableVector<T>*>(name.key())->value();
}

template <class T>
const QVector<T>* Model::pullValueVectorPtr(Identifier name) {
    return seekOneChild<VariableVector<T>*>(name.key())->valuePtr();
}

template <class T>
void Model::pushValueVector(Identifier name, const QVector<T> &value) {
    seekOneChild<ParameterVector<T>*>(name.key())->setValue(value);
}

} //namespace

#endif
