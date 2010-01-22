/* Copyright (C) 2009-2010 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_OBJECT_POOL_H
#define UNISIM_OBJECT_POOL_H

#include <QList>
#include <QMap>
#include <QObject>
#include "exception.h"
#include "identifier.h"

namespace UniSim{
	
class ObjectPool : public QObject
{
    Q_OBJECT
public:
    ObjectPool(QObject *parent=0);
    void attach(Identifier id, QObject *object);
    void detach(Identifier id);
    void erase(Identifier id);
    bool contains(Identifier id);
    void deferTo(ObjectPool *pool);
    template <class T> T find(UniSim::Identifier);
private:
    typedef QMap<Identifier, QObject*> ObjectMap;
    ObjectMap objects;
    ObjectPool *deferredPool;

    // Singleton
    ObjectPool();
    static ObjectPool* _objectPool;
    friend ObjectPool* objectPool();
};

template <class T> T ObjectPool::find(UniSim::Identifier id) {
    if (!objects.contains(id))
        throw UniSim::Exception("Cannot find object in object pool: " + id.label());
    else {
        T object = dynamic_cast<T>(objects.value(id));
        if (!object)
            throw UniSim::Exception("Object found in object pool '" + id.label() +
                                    "' is not of expected type");
        return object;
    }
}

inline ObjectPool* objectPool()
{
    if (!ObjectPool::_objectPool) {
        ObjectPool::_objectPool = new ObjectPool(0);
    }
    return ObjectPool::_objectPool->deferredPool ?
           ObjectPool::_objectPool->deferredPool :
           ObjectPool::_objectPool;
}

} //namespace

#endif
