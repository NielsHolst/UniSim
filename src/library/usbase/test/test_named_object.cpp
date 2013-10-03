#include <iostream>
#include <usbase/model.h>
#include <usbase/named_object.h>
#include <usbase/utilities.h>
#include <usbase/variable.h>
#include <usengine/simulation.h>
#include "test_named_object.h"

using namespace UniSim;

namespace {
    template <class T>
    T* create(QString name, QObject *parent) {
        T *obj = new T(name, parent);
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

void TestNamedObject::initTestCase() {
    animals = create<NamedObject>("animals",0);

    femaleAnimals = create<NamedObject>("female", animals);

    reptiles = create<NamedObject>("reptiles", animals);
    femaleReptiles = create<NamedObject>("female", reptiles);

    snakes = create<NamedObject>("snakes", reptiles);
    cobra = create<NamedObject>("cobra", snakes);
    lizards = create<NamedObject>("lizards", reptiles);
    mammals = create<NamedObject>("mammals", animals);
    mice = create<NamedObject>("mice", mammals);
    elephants = create<NamedObject>("elephants", mammals);
    femaleElephants = create<NamedObject>("female", elephants);

    dogs = create<NamedObject>("dogs", mammals);
    femaleDogs = create<NamedObject>("female", dogs);
    maleDogs = create<NamedObject>("male", dogs);
    new Variable<int>("size", &femaleDogsSize, femaleDogs, "desc");
    new Variable<int>("size", &maleDogsSize, maleDogs, "desc");
    new Variable<int>("weight", &femaleDogsWeight, femaleDogs, "desc");
    new Variable<int>("weight", &maleDogsWeight, maleDogs, "desc");

    dogsModel = create<Model>("dogs", mammals);
    femaleDogsModel = create<Model>("female", dogsModel);
    maleDogsModel = create<Model>("male", dogsModel);
//    writeObjectTree(animals);
}

void TestNamedObject::cleanupTestCase() {
    delete animals;
}

void TestNamedObject::testFindAtomicGloballyAll() {
    QList<NamedObject*> objects = animals->seekDescendants<NamedObject*>("*");
    QCOMPARE(objects.size(), 16);

    QList<Component*> components = animals->seekDescendants<Component*>("*");
    QCOMPARE(components.size(), 3);
}

void TestNamedObject::testFindAtomicGloballySome() {
    QList<NamedObject*> objects = animals->seekDescendants<NamedObject*>("dogs");
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(QSet<NamedObject*>() << dogs << dogsModel, objects) );

    QList<Model*> models = animals->seekDescendants<Model*>("dogs");
    QCOMPARE(models.size(), 1);
    QCOMPARE(models[0], dogsModel);
}

void TestNamedObject::testFindAtomicGloballyNone() {
    QList<QString*> strings;

    strings = animals->seekDescendants<QString*>("*");
    QCOMPARE(strings.size(), 0);

    strings = animals->seekDescendants<QString*>("dogs");
    QCOMPARE(strings.size(), 0);

    QList<NamedObject*> objects = animals->seekDescendants<NamedObject*>("cats");
    QCOMPARE(objects.size(), 0);
}

void TestNamedObject::testFindAtomicDescendantsAll() {
    QList<NamedObject*> objects = mammals->seekDescendants<NamedObject*>("*");
    QCOMPARE(objects.size(), 9);

    QList<Component*> components = mammals->seekDescendants<Component*>("*");
    QCOMPARE(components.size(), 3);
    QVERIFY( setEqualsList(QSet<Component*>() << dogsModel << maleDogsModel << femaleDogsModel, components) );
}

void TestNamedObject::testFindAtomicDescendantsSome() {
    QList<NamedObject*> objects = mammals->seekDescendants<NamedObject*>("female");
    QCOMPARE(objects.size(), 3);
    QVERIFY( setEqualsList(QSet<NamedObject*>() << femaleElephants << femaleDogs << femaleDogsModel, objects) );
}

void TestNamedObject::testFindAtomicDescendantsNone() {
    QList<NamedObject*> objects = reptiles->seekDescendants<NamedObject*>("cats");
    QCOMPARE(objects.size(), 0);
}

void TestNamedObject::testFindJokerFirst() {
    QList<NamedObject*> objects;
    QSet<NamedObject*> females;
    females << femaleAnimals << femaleReptiles << femaleElephants << femaleDogs << femaleDogsModel;

    objects = animals->seekDescendants<NamedObject*>("*/female");
    QCOMPARE(objects.size(), 5);
    QVERIFY( setEqualsList(females, objects) );

    females.remove(femaleAnimals);
    objects = animals->seekDescendants<NamedObject*>("*/*/female");
    QCOMPARE(objects.size(), 4);
    QVERIFY( setEqualsList(females, objects) );

    objects = animals->seekDescendants<NamedObject*>("*/animals");
    QCOMPARE(objects.size(), 0);

    objects = animals->seekDescendants<NamedObject*>("*/reptiles");
    QCOMPARE(objects.size(), 1);
    QCOMPARE(objects[0], reptiles);

    objects = animals->seekDescendants<NamedObject*>("*/*reptiles");
    QCOMPARE(objects.size(), 0);
}

void TestNamedObject::testFindJokerInside() {
    QList<NamedObject*> objects;
    QSet<NamedObject*> females;
    females << femaleElephants << femaleDogs << femaleDogsModel;

    objects = animals->seekDescendants<NamedObject*>("mammals/*/female");
    QCOMPARE(objects.size(), 3);
    QVERIFY( setEqualsList(females, objects) );

    objects = animals->seekDescendants<NamedObject*>("animals/*/*/female");
    QCOMPARE(objects.size(), 3);
    QVERIFY( setEqualsList(females, objects) );

    objects = animals->seekDescendants<NamedObject*>("animals/*/*/*/female");
    QCOMPARE(objects.size(), 0);
}

void TestNamedObject::testFindJokerLast() {
    QList<NamedObject*> objects;
    objects = animals->seekDescendants<NamedObject*>("animals/reptiles/*");
    QCOMPARE(objects.size(), 3);
    QVERIFY( setEqualsList(QSet<NamedObject*>() << femaleReptiles << snakes << lizards, objects) );

    objects = animals->seekDescendants<NamedObject*>("mammals/*/*");
    QCOMPARE(objects.size(), 5);
    QVERIFY( setEqualsList(QSet<NamedObject*>() << femaleElephants << femaleDogs << maleDogs
                           << femaleDogsModel << maleDogsModel, objects) );

    objects = animals->seekDescendants<NamedObject*>("dogs/*/*/*");
    QCOMPARE(objects.size(), 0);
}

void TestNamedObject::testFindSlashes() {
    QList<Model*> models;
    try {
        models = animals->seekDescendants<Model*>("/dogs/female");
        QCOMPARE(models.size(), 1);
        QCOMPARE(models[0], femaleDogsModel);

        models = animals->seekDescendants<Model*>("/dogs/female/");
        QCOMPARE(models.size(), 1);
        QCOMPARE(models[0], femaleDogsModel);

        models = animals->seekDescendants<Model*>("dogs/female/");
        QCOMPARE(models.size(), 1);
        QCOMPARE(models[0], femaleDogsModel);
    }
    catch (Exception &ex) {
        QVERIFY(false);
    }
}

void TestNamedObject::testFindExceptions() {
    QList<NamedObject*> objects;

    bool excepted = false;
    try {
        objects = animals->seekDescendants<NamedObject*>("///dogs/female");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        objects = animals->seekDescendants<NamedObject*>("dogs//female");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        objects = animals->seekDescendants<NamedObject*>("dogs/female//");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestNamedObject::testFindEmpty() {
    QList<NamedObject*> objects;
    objects = animals->seekDescendants<NamedObject*>("");
    QCOMPARE(objects.size(), 0);

    objects = animals->seekDescendants<NamedObject*>("/");
    QCOMPARE(objects.size(), 0);
}

void TestNamedObject::testFindChildrenFromRoot() {
    QList<NamedObject*> objects;
    objects = animals->seekChildren<NamedObject*>("*");
    QCOMPARE(objects.size(), 3);
    QVERIFY( setEqualsList(QSet<NamedObject*>() << femaleAnimals << mammals << reptiles, objects) );
    objects = animals->seekChildren<NamedObject*>("mammals");
    QCOMPARE(objects.size(), 1);
    QCOMPARE(objects[0], mammals);

    objects = animals->seekChildren<NamedObject*>("insects");
    QCOMPARE(objects.size(), 0);

    QList<QString*> strings = animals->seekChildren<QString*>("mammals");
    QCOMPARE(strings.size(), 0);
}

void TestNamedObject::testFindChildrenFromParent() {
    QList<NamedObject*> objects;

    objects = mammals->seekChildren<NamedObject*>("*");
    QCOMPARE(objects.size(), 4);
    QVERIFY( setEqualsList(QSet<NamedObject*>() << mice << elephants << dogs << dogsModel, objects) );

    QList<Model*> models = mammals->seekChildren<Model*>("*");
    QCOMPARE(models.size(), 1);
    QCOMPARE(models[0], dogsModel);

    objects = mammals->seekChildren<NamedObject*>("mice");
    QCOMPARE(objects.size(), 1);
    QCOMPARE(objects[0], mice);

    objects = mammals->seekChildren<NamedObject*>("bats");
    QCOMPARE(objects.size(), 0);

    QList<QString*> strings = animals->seekChildren<QString*>("mice");
    QCOMPARE(strings.size(), 0);
}

void TestNamedObject::testFindChildFromRoot() {
    NamedObject* object;
    bool excepted;

    excepted = false;
    try {
        object = animals->seekOneChild<NamedObject*>("*");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        object = animals->seekOneChild<NamedObject*>("mammals");
        QCOMPARE(object, mammals);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(!excepted);
}

void TestNamedObject::testFindChildFromParent() {
    NamedObject* object;
    bool excepted;

    excepted = false;
    try {
        object = mammals->seekOneChild<NamedObject*>("*");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        object = mammals->seekOneChild<NamedObject*>("mice");
        QCOMPARE(object, mice);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(!excepted);
}

void TestNamedObject::testSeekChildAndParentNone() {
    bool excepted = false;
    try {
        animals->seekOne<NamedObject*, Variable<int>*>("dogs[size]");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestNamedObject::testSeekChildAndParentOne() {
    Variable<int>* found;
    try  {
        found = animals->seekOne<NamedObject*, Variable<int>*>("dogs/female[size]");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Could not find one child and parent: " + ex.message()));
    }
    QCOMPARE(found->valuePtr(), &femaleDogsSize);
}

void TestNamedObject::testSeekChildrenAndParentsMany() {
    QList<Variable<int>*> found;
    found = animals->seekMany<NamedObject*, Variable<int>*>("dogs/*[size]");
    QCOMPARE(found.size(), 2);
    bool match1 = found[0]->valuePtr()==&femaleDogsSize && found[1]->valuePtr()==&maleDogsSize;
    bool match2 = found[1]->valuePtr()==&femaleDogsSize && found[0]->valuePtr()==&maleDogsSize;
    QVERIFY(match1 || match2);
}

void TestNamedObject::testSeekChildrenAndParentsJoker() {
    QList<Variable<int>*> found;
    try  {
        found = animals->seekMany<NamedObject*, Variable<int>*>("dogs/female[*]");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Could not find one child and parent: " + ex.message()));
    }
    QCOMPARE(found.size(), 2);
    bool match1 = found[0]->valuePtr()==&femaleDogsSize && found[1]->valuePtr()==&femaleDogsWeight;
    bool match2 = found[1]->valuePtr()==&femaleDogsSize && found[0]->valuePtr()==&femaleDogsWeight;
    QVERIFY(match1 || match2);
}

void TestNamedObject::testPeekOne() {
    NamedObject *animals2 = animals->peekOne<NamedObject*>("animals");
    QVERIFY(!animals2);
    NamedObject *mice2 = animals->peekOne<NamedObject*>("mice");
    QVERIFY(mice2);
    NamedObject *rats2 = animals->peekOne<NamedObject*>("rats");
    QVERIFY(!rats2);
}

void TestNamedObject::testPeekOneChild() {
    NamedObject *mice2 = mammals->peekOneChild<NamedObject*>("mice");
    QVERIFY(mice2);
    NamedObject *rats2 = mammals->peekOneChild<NamedObject*>("rats");
    QVERIFY(!rats2);
}

void TestNamedObject::testPeekOneDescendant() {
    NamedObject *mice2 = mammals->peekOneDescendant<NamedObject*>("mice");
    QVERIFY(mice2);
    NamedObject *rats2 = mammals->peekOneDescendant<NamedObject*>("rats");
    QVERIFY(!rats2);
}

void TestNamedObject::testPeekOneAscendant() {
    NamedObject *animals2 = mice->peekOneAscendant<NamedObject*>("animals");
    QCOMPARE(animals, animals2);
    NamedObject *mammals2 = mice->peekOneAscendant<NamedObject*>("mammals");
    QVERIFY(mammals2);
    NamedObject *reptiles2 = mice->peekOneAscendant<NamedObject*>("reptiles");
    QVERIFY(!reptiles2);
}


void TestNamedObject::testPeekOneNearest() {
    NamedObject *femaleElephants2 = elephants->peekOneNearest<NamedObject*>("female");
    QCOMPARE(femaleElephants, femaleElephants2);

    NamedObject *femaleAnimals2 = mice->peekOneNearest<NamedObject*>("female");
    QCOMPARE(femaleAnimals, femaleAnimals2);

    NamedObject *femaleReptiles2 = snakes->peekOneNearest<NamedObject*>("female");
    QCOMPARE(femaleReptiles, femaleReptiles2);

    NamedObject *maleElephants2 = elephants->peekOneNearest<NamedObject*>("male");
    QVERIFY(!maleElephants2);
}

void TestNamedObject::testSeekOneNearest() {
    NamedObject *femaleElephants2 = elephants->seekOneNearest<NamedObject*>("female");
    QCOMPARE(femaleElephants, femaleElephants2);

    NamedObject *femaleAnimals2 = mice->seekOneNearest<NamedObject*>("female");
    QCOMPARE(femaleAnimals, femaleAnimals2);

    NamedObject *femaleReptiles2 = snakes->seekOneNearest<NamedObject*>("female");
    QCOMPARE(femaleReptiles, femaleReptiles2);

    bool excepted = false;
    try {
        animals->seekOneNearest<NamedObject*>("male");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestNamedObject::testPeekParent() {
    NamedObject *ofRoot = animals->peekParent<NamedObject*>("*");
    QVERIFY(ofRoot == 0);

    NamedObject *ofSnakesModel = snakes->peekParent<Model*>("reptiles");
    QVERIFY(ofSnakesModel == 0);

    NamedObject *ofSnakes = snakes->peekParent<NamedObject*>("reptiles");
    QCOMPARE(ofSnakes, reptiles);

    NamedObject *ofFemaleDogsModel = femaleDogsModel->peekParent<Model*>("*");
    QCOMPARE(ofFemaleDogsModel, dogsModel);
}

void TestNamedObject::testSeekParent() {
    bool excepted = false;
    try {
        animals->seekParent<NamedObject*>("*");
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    NamedObject *ofFemaleDogsModel = femaleDogsModel->seekParent<Model*>("*");
    QCOMPARE(ofFemaleDogsModel, dogsModel);
}

void TestNamedObject::testRelativeSelf() {
    QString path = dogs->absolutePath("./female[size]");
    QCOMPARE(path.toLower(), QString("/animals/mammals/dogs/female[size]"));
}

void TestNamedObject::testRelativeSelfBare() {
    QString path = dogs->absolutePath(".[size]");
    QCOMPARE(path.toLower(), QString("/animals/mammals/dogs[size]"));
}

void TestNamedObject::testRelativeSibling() {
    QString path = dogs->absolutePath("../elephants/female[size]");
    QCOMPARE(path.toLower(), QString("/animals/mammals/elephants/female[size]"));
}

void TestNamedObject::testRelativeSiblingBare() {
    QString path = dogs->absolutePath("..[size]");
    QCOMPARE(path.toLower(), QString("/animals/mammals[size]"));
}

void TestNamedObject::testRelativeFromRoot() {
    QString path = animals->absolutePath("./reptiles[size]");
    QCOMPARE(path.toLower(), QString("/animals/reptiles[size]"));
}

void TestNamedObject::testRelativeAtRoot() {
    QString path = reptiles->absolutePath("../mammals[size]");
}

void TestNamedObject::testRelativeAtRootBare() {
    QString path = reptiles->absolutePath("..[size]");
    QCOMPARE(path.toLower(), QString("/animals[size]"));
}

void TestNamedObject::testRelativeBeyondRoot() {
    bool excepted = false;
    try {
        reptiles->absolutePath("../..[size]");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestNamedObject::testRelativeNot() {
    QString path = dogs->absolutePath("elephants/female[size]");
    QCOMPARE(path.toLower(), QString("elephants/female[size]"));
}

void TestNamedObject::testRelativeSeek() {
    Variable<int>* found;
    try  {
        found = elephants->seekOne<NamedObject*, Variable<int>*>("../dogs/female[size]");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable("Could not find one child and parent: " + ex.message()));
    }
    QCOMPARE(found->valuePtr(), &femaleDogsSize);
}
