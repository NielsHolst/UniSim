/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <iostream>
#include "model.h"

using namespace std;

namespace UniSim{
	
Model::Model(Identifier name, QObject *parent)
    : Component(name, parent), _hide(false), prev(0), next(0)
{
}

Identifier Model::classId() {
    QVariant label = property("classLabel");
    Q_ASSERT(label.isValid());
    return Identifier(label.toString());
}

namespace {
    QString peekIdOrParameter(Identifier key, Model *model) {
        if (key == model->classId())
            return model->id().label();

        QList<VariableBase*> parameter = model->seekChildren<VariableBase*>(key.label());
        if (parameter.size() == 1)
            return parameter.value(0)->toString();
        Q_ASSERT(parameter.isEmpty());
        return QString();
    }
}

QString Model::peekKeyValue(Identifier key) {
    QString result = peekIdOrParameter(key, this);
    if (!result.isEmpty())
        return result;

    Model *ascendant = peekParent<Model*>("*");
    while (ascendant) {
        result = peekIdOrParameter(key, ascendant);
        if (!result.isEmpty())
            return result;
        ascendant = ascendant->peekParent<Model*>("*");
    }
    return QString();
}

bool Model::hide() const {
    return _hide;
}

void Model::setHide(bool value) {
    _hide = value;
}

bool Model::hasInstances() {
    return next!=0 || prev!=0;
}

void Model::setInstances(Model *prev_, Model *next_) {
    Q_ASSERT(prev_!=this && next_!=this);
    prev = prev_;
    next = next_;
}

} //namespace

