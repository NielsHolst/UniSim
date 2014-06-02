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
#include "reference.h"
#include "variable.h"
#include "variable_base.h"
#include "variable_vector.h"

namespace UniSim{

QList<Ref*> Ref::all;

Ref::Ref(NamedObject *_parameterParent, ParameterBase *_parameter, QString _reference)
    : parameterParent(_parameterParent), parameter(_parameter), reference(_reference),
      source(0)
{
    all << this;
}

void Ref::clear() {
    for (int i = 0; i < all.size(); ++i)
        delete all[i];
    all.clear();
}

void Ref::remove(ParameterBase *theParameter) {
    for (int i = 0; i < all.size(); ++i) {
        if (all.at(i)->parameter == theParameter) {
            delete all.takeAt(i);
            break;
        }
    }
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
        try {
            ref->source = ref->parameterParent->seekOne<NamedObject*, VariableBase*>(ref->reference);
        }
        catch (Exception &ex) {
            throw Exception(ref->notFoundMessage(ex));
        }
        bool coupled =
            couple<bool>(ref->parameter, ref->source) ||
            couple<char>(ref->parameter, ref->source) ||
            couple<int>(ref->parameter, ref->source) ||
            couple<long>(ref->parameter, ref->source) ||
            couple<unsigned>(ref->parameter, ref->source) ||
            couple<float>(ref->parameter, ref->source) ||
            couple<double>(ref->parameter, ref->source) ||
            couple<QTime>(ref->parameter, ref->source) ||
            couple<QDate>(ref->parameter, ref->source) ||
            couple<QString>(ref->parameter, ref->source);
        if (!coupled) {
            QString msg("The type of the value referred to by '%1' does not match that of the parameter");
            throw Exception(msg.arg(ref->reference), ref->parameterParent);
        }
    }
}

namespace {
    template<class ChildType>
    QStringList lookupChildren(NamedObject *parent) {
        auto children = parent->seekChildren<ChildType>("*");
        QStringList names;
        for (auto child : children) {
            names << "'" + child->id().label() + "'";
        }
        return names;
    }
}

QString Ref::notFoundMessage(Exception &ex) {
    QString from = QString("referenced from '%1'").arg(parameterParent->fullLabel());
    Reference ref{reference, parameterParent};
    QString path = ref.path(),
            name = ref.name();

//    QStringList names = splitParentChildExpression(reference, parameterParent);
    QString absPath = parameterParent->absolutePath(path);
//    NamedObject *model = parameterParent->peekOne<NamedObject*>(absPath);
    QList<NamedObject*> models = parameterParent->seekMany<NamedObject*>(absPath);



    // No model found
    if (models.isEmpty()) {
        QString msg = "Exception: %1\n\nError in reference '%2' %3: Model '%4' not found";
        return msg.arg(ex.message()).arg(reference).arg(from).arg(path);
    }
    // More than one model found
    else if (models.size() > 1) {
        QString msg = "Exception: %1\n\nMore than one model found matching '%2':\n%3";
        QString names;
        for (auto model : models)
            names += model->fullLabel() + "\n";
        return msg.arg(ex.message()).arg(reference).arg(names);
    }
    NamedObject *model = models[0];
    // No variable found
    QString msg = "Exception: %1\n\nError in reference '%2' %3: Parameter named '%4' not found.\n"
            "Model '%5' contains these parameters and variables:\n%6";
    QStringList vars = lookupChildren<VariableBase*>(model);
    msg = msg.arg(ex.message()).arg(reference).arg(from).arg(name).arg(path).arg(vars.join("\n"));
    if (vars.isEmpty())
        msg += "None!";
    return msg;
}


void Ref::writeEdges(QFile &f) {
    f.write("edge [color=red];");
    for (int i = 0; i < all.size(); ++i) {
        Ref *ref = all[i];
        Q_ASSERT(ref->source);
        const NamedObject *from = ref->source->parent();
        Q_ASSERT(from);
        QString line = QString("%1->%2;\n")
                .arg(from->uniqueId())
                .arg(ref->parameterParent->uniqueId());
        f.write(qPrintable(line));
    }
}

} //namespace
