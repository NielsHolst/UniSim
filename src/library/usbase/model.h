/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include "parameter.h"
#include "pull_variable.h"
#include "push_variable.h"
#include "utilities.h"

namespace UniSim{
	
class Model : public UniSim::Component
{
	Q_OBJECT
public:
    Model(Identifier name, QObject *parent=0);

    template <class T> T parameter(Identifier name);
    template <class T> void pushVariable(Identifier name, T value);
    template <class T> T pullVariable(Identifier name);
    template <class T> const T* pullVariablePtr(Identifier name);

    bool hide() const;
    void setHide(bool value);
private:
    bool _hide;
};

typedef QList<Model*> Models;

template <class T>
T Model::parameter(Identifier name)
{
    ParameterBase* basePar = seekOneChild<ParameterBase*>(name.key());
    QVariant variant = basePar->toVariant();
    if (!variant.canConvert<T>())
        throw Exception("Cannot convert parameter '" + name.label() +
                        "' to type " + QVariant(T()).typeName(), this);
    return variant.value<T>();
}


template <class T>
void Model::pushVariable(Identifier name, T value)
{
    seekOneChild<PushVariable<T>*>(name.key())->setValue(value);
}

template <class T>
T Model::pullVariable(Identifier name)
{
    PullVariableBase* baseVar = seekOneChild<PullVariableBase*>(name.key());
    QVariant variant = baseVar->toVariant();
    return variant.value<T>();
}

template <class T>
const T* Model::pullVariablePtr(Identifier name)
{
    return seekOneChild<PullVariable<T>*>(name.key())->valuePtr();
}

} //namespace

#endif
