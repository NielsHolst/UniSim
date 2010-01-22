#include <iostream>
#include <QSet>
#include <usbase/component.h>
#include <usbase/model.h>
#include <usbase/utilities.h>
#include "test_utilities.h"

using namespace UniSim;

void TestUtilities::testSplitAtNamespace()
{
    QString namespacePart, ownNamePart;

    splitAtNamespace("One::two", &namespacePart, &ownNamePart);
    QCOMPARE(namespacePart, QString("One"));
    QCOMPARE(ownNamePart, QString("two"));

    splitAtNamespace("One::two::Three", &namespacePart, &ownNamePart);
    QCOMPARE(namespacePart, QString("One::two"));
    QCOMPARE(ownNamePart, QString("Three"));

    splitAtNamespace("one", &namespacePart, &ownNamePart);
    QCOMPARE(namespacePart, QString());
    QCOMPARE(ownNamePart, QString("one"));

    splitAtNamespace("::one", &namespacePart, &ownNamePart);
    QCOMPARE(namespacePart, QString());
    QCOMPARE(ownNamePart, QString("one"));

    splitAtNamespace("one::", &namespacePart, &ownNamePart);
    QCOMPARE(namespacePart, QString("one"));
    QCOMPARE(ownNamePart, QString(""));
}

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

void TestUtilities::initTestCase() {
    animals = create<QObject>("animals",0);
    setSimulationObject(animals);

    reptiles = create<QObject>("reptiles", animals);
    snakes = create<QObject>("snakes", reptiles);
    cobra = create<QObject>("cobra", snakes);
    lizards = create<QObject>("lizards", reptiles);
    mammals = create<QObject>("mammals", animals);
    mice = create<QObject>("mice", mammals);
    elephants = create<QObject>("elephants", mammals);

    dogs = create<QObject>("dogs", mammals);
    femaleDogs = create<QObject>("female", dogs);
    maleDogs = create<QObject>("male", dogs);

    dogsModel = create<Model>("dogs", mammals);
    femaleDogsModel = create<Model>("female", dogsModel);
    maleDogsModel = create<Model>("male", dogsModel);
}

void TestUtilities::cleanupTestCase() {
    delete animals;
}

void TestUtilities::testFindAtomicGloballyAll() {
    QList<QObject*> objects = find<QObject*>("*");
    QCOMPARE(objects.size(), 14);

    QList<Component*> components = find<Component*>("*");
    QCOMPARE(components.size(), 3);
}

void TestUtilities::testFindAtomicGloballySome() {
    QList<QObject*> objects = find<QObject*>("dogs");
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(QSet<QObject*>() << dogs << dogsModel, objects) );

    QList<Model*> models = find<Model*>("dogs");
    QCOMPARE(models.size(), 1);
    QCOMPARE(models[0], dogsModel);
}

void TestUtilities::testFindAtomicGloballyNone() {
    QList<QString*> strings;

    strings = find<QString*>("*");
    QCOMPARE(strings.size(), 0);

    strings = find<QString*>("dogs");
    QCOMPARE(strings.size(), 0);

    QList<QObject*> objects = find<QObject*>("cats");
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testFindAtomicDescendantsAll() {
    QList<QObject*> objects = find<QObject*>("*", mammals);
    QCOMPARE(objects.size(), 8);

    QList<Component*> components = find<Component*>("*", mammals);
    QCOMPARE(components.size(), 3);
    QVERIFY( setEqualsList(QSet<Component*>() << dogsModel << maleDogsModel << femaleDogsModel, components) );
}

void TestUtilities::testFindAtomicDescendantsSome() {
    QList<QObject*> objects = find<QObject*>("female", mammals);
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(QSet<QObject*>() << femaleDogs << femaleDogsModel, objects) );
}

void TestUtilities::testFindAtomicDescendantsNone() {
    QList<QObject*> objects = find<QObject*>("cats", reptiles);
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testFindJokerFirst() {
    QList<QObject*> objects;
    QSet<QObject*> females;
    females << femaleDogs << femaleDogsModel;

    objects = find<QObject*>("*/female");
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(females, objects) );

    objects = find<QObject*>("*/*/female");
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(females, objects) );

    objects = find<QObject*>("*/animals");
    QCOMPARE(objects.size(), 0);

    objects = find<QObject*>("*/reptiles");
    QCOMPARE(objects.size(), 1);
    QCOMPARE(objects[0], reptiles);

    objects = find<QObject*>("*/*reptiles");
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testFindJokerInside() {
    QList<QObject*> objects;
    QSet<QObject*> females;
    females << femaleDogs << femaleDogsModel;

    objects = find<QObject*>("mammals/*/female");
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(females, objects) );

    objects = find<QObject*>("animals/*/*/female");
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(females, objects) );

    objects = find<QObject*>("animals/*/*/*/female");
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testFindJokerLast() {
    QList<QObject*> objects;
    objects = find<QObject*>("animals/reptiles/*");
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(QSet<QObject*>() << snakes << lizards, objects) );

    objects = find<QObject*>("mammals/*/*");
    QCOMPARE(objects.size(), 4);

    objects = find<QObject*>("dogs/*/*");
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testQStringSplit() {
    QString str = ",a,,b,c";
    QStringList list = str.split(",");
    QCOMPARE(list.size(), 5);
    QVERIFY(list[0].isEmpty());
    QCOMPARE(list[1], QString("a"));
    QVERIFY(list[2].isEmpty());
}

void TestUtilities::testFindSlashes() {
    QList<Model*> models;
    try {
        models = find<Model*>("/dogs/female");
        QCOMPARE(models.size(), 1);
        QCOMPARE(models[0], femaleDogsModel);

        models = find<Model*>("/dogs/female/");
        QCOMPARE(models.size(), 1);
        QCOMPARE(models[0], femaleDogsModel);

        models = find<Model*>("dogs/female/");
        QCOMPARE(models.size(), 1);
        QCOMPARE(models[0], femaleDogsModel);
    }
    catch (Exception &ex) {
        QVERIFY(false);
    }
}

void TestUtilities::testFindExceptions() {
    QList<QObject*> objects;

    bool excepted = false;
    try {
        objects = find<QObject*>("///dogs/female");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        objects = find<QObject*>("dogs//female");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        objects = find<QObject*>("dogs/female//");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestUtilities::testFindEmpty() {
    QList<QObject*> objects;
    objects = find<QObject*>("");
    QCOMPARE(objects.size(), 0);

    objects = find<QObject*>("/");
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testFindChildrenFromRoot() {
    QList<QObject*> objects;
    objects = UniSim::findChildren<QObject*>("*");
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(QSet<QObject*>() << mammals << reptiles, objects) );
    objects = UniSim::findChildren<QObject*>("mammals");
    QCOMPARE(objects.size(), 1);
    QCOMPARE(objects[0], mammals);

    objects = UniSim::findChildren<QObject*>("insects");
    QCOMPARE(objects.size(), 0);

    QList<QString*> strings = UniSim::findChildren<QString*>("mammals");
    QCOMPARE(strings.size(), 0);
}

void TestUtilities::testFindChildrenFromParent() {
    QList<QObject*> objects;

    objects = UniSim::findChildren<QObject*>("*", mammals);
    QCOMPARE(objects.size(), 4);
    QVERIFY( setEqualsList(QSet<QObject*>() << mice << elephants << dogs << dogsModel, objects) );

    QList<Model*> models = UniSim::findChildren<Model*>("*", mammals);
    QCOMPARE(models.size(), 1);
    QCOMPARE(models[0], dogsModel);

    objects = UniSim::findChildren<QObject*>("mice", mammals);
    QCOMPARE(objects.size(), 1);
    QCOMPARE(objects[0], mice);

    objects = UniSim::findChildren<QObject*>("bats", mammals);
    QCOMPARE(objects.size(), 0);

    QList<QString*> strings = UniSim::findChildren<QString*>("mice");
    QCOMPARE(strings.size(), 0);
}

void TestUtilities::testFindChildFromRoot() {
    QObject* object;
    bool excepted;

    excepted = false;
    try {
        object = UniSim::findChild<QObject*>("*");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        object = UniSim::findChild<QObject*>("mammals");
        QCOMPARE(object, mammals);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(!excepted);
}

void TestUtilities::testFindChildFromParent() {
    QObject* object;
    bool excepted;

    excepted = false;
    try {
        object = UniSim::findChild<QObject*>("*", mammals);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        object = UniSim::findChild<QObject*>("mice", mammals);
        QCOMPARE(object, mice);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(!excepted);
}
