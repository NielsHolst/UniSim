#ifndef UNISIM_TEST_ANIMALS_H
#define UNISIM_TEST_ANIMALS_H

#include <QObject>
#include "../utilities.h"

namespace animals {

    namespace {
        template <class T>
        T* create(QString name, QObject *parent) {
            T *obj = new T(name, parent);
            return obj;
        }

        template <>
        QObject* create<QObject>(QString name, QObject *parent) {
            QObject *obj = new QObject(parent);
            obj->setObjectName(name);
            return obj;
        }

        template <class T>
        bool setEqualsList(const QSet<T*> &set, const QList<T*> &list) {
            QSet<T*> set2;
            for (int i = 0; i < list.size(); ++i)
                set2 << list.at(i);
            return set == set2;
        }
    }

    QObject
        *theAnimals,
        *reptiles, *snakes, *cobra, *lizards,
        *mammals, *mice, *elephants, *dogs, *femaleDogs, *maleDogs;

    template <class T>
    void createAnimals<T>() {
        theAnimals = create<T>("animals",0);
        UniSim::setSimulationObject(theAnimals);

        reptiles = create<T>("reptiles", theAnimals);
        snakes = create<T>("snakes", reptiles);
        cobra = create<T>("cobra", snakes);
        lizards = create<T>("lizards", reptiles);
        mammals = create<T>("mammals", theAnimals);
        mice = create<T>("mice", mammals);
        elephants = create<T>("elephants", mammals);

        dogs = create<T>("dogs", mammals);
        femaleDogs = create<T>("female", dogs);
        maleDogs = create<T>("male", dogs);
    }

} //namespace

#endif
