/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QMessageBox>
#include <QTextStream>
#include "object_pool.h"

namespace UniSim{

// Singleton

ObjectPool* ObjectPool::_objectPool = 0;

ObjectPool* objectPool() {
    if (!ObjectPool::_objectPool) {
        ObjectPool::_objectPool = new ObjectPool(0);
    }
    return ObjectPool::_objectPool->deferredPool ?
           ObjectPool::_objectPool->deferredPool :
           ObjectPool::_objectPool;
}


// Class

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
    if (pool != this)
        deferredPool = pool;
}

QString ObjectPool::diagnose() {
    QString result;
    QTextStream s(&result);
    s << "ObjectPool at " << this << " is ";
    if (deferredPool)
        s << "deferring to ObjectPool at " << deferredPool;
    else
        s << "is not deferred";

    s << "\nParent: ";
    if (parent())
        s << "at " << parent() << "\n";
    else
        s << "None\n";

    s << "Attached objects:\n";
    if (objects.isEmpty())
        s << "None\n";
    else {
        ObjectIterator it(objects);
        while (it.hasNext()) {
            it.next();
            s << it.key().label() << " at " << it.value() << "\n";
        }
    }
    return result;
}

} //namespace

