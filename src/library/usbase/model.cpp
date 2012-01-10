/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/

#include "model.h"
#include "pull_variable.h"
#include "push_variable.h"

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

QString Model::peekKeyValue(Identifier key) {
    if (key == classId())
        return id().label();

    Model *ascendant = peekParent<Model*>("*");
    while (ascendant) {
        if (key == ascendant->classId())
            return ascendant->id().label();

        QList<ParameterBase*> parameter = ascendant->seekChildren<ParameterBase*>(key.label());
        if (parameter.size() == 1)
            return parameter.value(0)->toString();
        Q_ASSERT(parameter.isEmpty());
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

