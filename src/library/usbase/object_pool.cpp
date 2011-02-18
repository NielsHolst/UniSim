/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "object_pool.h"

namespace UniSim{

ObjectPool* ObjectPool::_objectPool = 0;

ObjectPool::ObjectPool(QObject *parent)
        : QObject(parent), deferredPool(0) {
}

void ObjectPool::attach(Identifier id, QObject *object) {
    objects[id] = object;
}


void ObjectPool::detach(Identifier id) {
    if (!contains(id)) return;
    objects.remove(id);
}

void ObjectPool::erase(Identifier id) {
    if (!contains(id)) return;
    QObject *object = find<QObject*>(id);
    Q_ASSERT(!object->isWidgetType());
    delete object;
    objects.remove(id);
}

bool ObjectPool::contains(Identifier id) {
    return objects.contains(id);
}

void ObjectPool::deferTo(ObjectPool *pool) {
    deferredPool = pool;
}

} //namespace

