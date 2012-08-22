/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
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
#include "variable.h"

namespace UniSim{

//template<class T> class Parameter;
//template<class T> class Variable;

class Model : public UniSim::Component
{
	Q_OBJECT
public:
    Model(Identifier name, QObject *parent=0);

    template <class T> T pullValue(Identifier name);
    template <class T> const T* pullValuePtr(Identifier name);
    template <class T> void pushValue(Identifier name, T value);

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
void Model::pushValue(Identifier name, T value)
{
    seekOneChild<Parameter<T>*>(name.key())->setValue(value);
}

} //namespace

#endif
