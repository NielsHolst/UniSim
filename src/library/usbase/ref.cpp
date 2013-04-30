/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "named_object.h"
#include "parameter.h"
#include "parameter_base.h"
#include "parameter_vector.h"
#include "ref.h"
#include "variable.h"
#include "variable_base.h"
#include "variable_vector.h"

namespace UniSim{

QList<Ref*> Ref::all;

Ref::Ref(NamedObject *_parameterParent, ParameterBase *_parameter, QString _reference)
    : parameterParent(_parameterParent), parameter(_parameter), reference(_reference)
{
    all << this;
}

void Ref::clear() {
    for (int i = 0; i < all.size(); ++i)
        delete all[i];
    all.clear();
}

namespace {

    template <class ParameterT, class VariableT, class ValuePtrT>
    bool baseCouple(ParameterBase *parameter, const VariableBase *variable)
    {
        ParameterT* parameterTyped = dynamic_cast<ParameterT*>(parameter);
        const VariableT *variableTyped = dynamic_cast<const VariableT*>(variable);
        bool matchedT = parameterTyped && variableTyped;
        if (matchedT) {
            const ValuePtrT *redirectTo = variableTyped->valuePtr();
            parameterTyped->redirectValuePtr(redirectTo);
        }
        return matchedT;
    }


    template <class T>
    bool couple(ParameterBase *parameter, const VariableBase *variable)
    {
        return baseCouple<Parameter<T>, Variable<T>, T >(parameter, variable) ||
               baseCouple<ParameterVector<T>, VariableVector<T>, QVector<T> >(parameter, variable);
    }

}

void Ref::resolve() {
    for (int i = 0; i < all.size(); ++i) {
        Ref *ref = all[i];
        const VariableBase *source =
                ref->parameterParent->seekOne<NamedObject*, VariableBase*>(ref->reference);
        bool coupled =
            couple<bool>(ref->parameter, source) ||
            couple<int>(ref->parameter, source) ||
            couple<long>(ref->parameter, source) ||
            couple<unsigned>(ref->parameter, source) ||
            couple<float>(ref->parameter, source) ||
            couple<double>(ref->parameter, source) ||
            couple<QDate>(ref->parameter, source) ||
            couple<QString>(ref->parameter, source);
        if (!coupled) {
            QString msg("The type of the value referred to '%1' does not match that of the parameter");
            throw Exception(msg.arg(ref->reference), ref->parameterParent);
        }
    }
    clear();
}

} //namespace
