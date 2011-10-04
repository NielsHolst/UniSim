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

