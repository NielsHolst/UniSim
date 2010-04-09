#include <iostream>
#include <QSet>
#include <QStringList>
#include <usbase/component.h>
#include <usbase/model.h>
#include <usbase/utilities.h>
#include "test_utilities.h"

using namespace UniSim;


void TestUtilities::testDecodeSimpleListEmpty() {
    QStringList list;
    try {
        list = decodeSimpleList("");
        QVERIFY(list.isEmpty());

        list = decodeSimpleList("()");
        QVERIFY(list.isEmpty());

        list = decodeSimpleList("( )");
        QVERIFY(list.isEmpty());
    }
    catch (Exception &ex) {
        QString msg = "Wrong diagnose: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestUtilities::testDecodeSimpleListOne() {
    QStringList list;
    try {
        list = decodeSimpleList("abc");
        QCOMPARE(list.size(), 1);
        QCOMPARE(list.value(0), QString("abc"));

        list = decodeSimpleList("(abc)");
        QCOMPARE(list.size(), 1);
        QCOMPARE(list.value(0), QString("abc"));

        list = decodeSimpleList("a b c");
        QCOMPARE(list.size(), 1);
        QCOMPARE(list.value(0), QString("a b c"));
    }
    catch (Exception &ex) {
        QString msg = "Wrong diagnose: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestUtilities::testDecodeSimpleListMany() {
    QStringList list;

    try {
        list = decodeSimpleList("(a b c)");
        QCOMPARE(list.size(), 3);
        QCOMPARE(list.value(0), QString("a"));
        QCOMPARE(list.value(1), QString("b"));
        QCOMPARE(list.value(2), QString("c"));

        list = decodeSimpleList(" ( a  b  c  ) ");
        QCOMPARE(list.size(), 3);
        QCOMPARE(list.value(0), QString("a"));
        QCOMPARE(list.value(1), QString("b"));
        QCOMPARE(list.value(2), QString("c"));
    }
    catch (Exception &ex) {
        QString msg = "Wrong diagnose: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestUtilities::testDecodeSimpleListFaulty() {
    QStringList list;

    try {
        list = decodeSimpleList("(");
        QFAIL("Faulty list not diagnosed");
    }
    catch (Exception &ex) {
    }

    try {
        list = decodeSimpleList(")");
        QFAIL("Faulty list not diagnosed");
    }
    catch (Exception &ex) {
    }

    try {
        list = decodeSimpleList("(a b c");
        QFAIL("Faulty list not diagnosed");
    }
    catch (Exception &ex) {    
    }

    try {
        list = decodeSimpleList("a b c)");
        QFAIL("Faulty list not diagnosed");
    }
    catch (Exception &ex) {
    }
}


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
    QList<QObject*> objects = seekDescendants<QObject*>("*", 0);
    QCOMPARE(objects.size(), 14);

    QList<Component*> components = seekDescendants<Component*>("*", 0);
    QCOMPARE(components.size(), 3);
}

void TestUtilities::testFindAtomicGloballySome() {
    QList<QObject*> objects = seekDescendants<QObject*>("dogs", 0);
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(QSet<QObject*>() << dogs << dogsModel, objects) );

    QList<Model*> models = seekDescendants<Model*>("dogs", 0);
    QCOMPARE(models.size(), 1);
    QCOMPARE(models[0], dogsModel);
}

void TestUtilities::testFindAtomicGloballyNone() {
    QList<QString*> strings;

    strings = seekDescendants<QString*>("*", 0);
    QCOMPARE(strings.size(), 0);

    strings = seekDescendants<QString*>("dogs", 0);
    QCOMPARE(strings.size(), 0);

    QList<QObject*> objects = seekDescendants<QObject*>("cats", 0);
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testFindAtomicDescendantsAll() {
    QList<QObject*> objects = seekDescendants<QObject*>("*", mammals);
    QCOMPARE(objects.size(), 8);

    QList<Component*> components = seekDescendants<Component*>("*", mammals);
    QCOMPARE(components.size(), 3);
    QVERIFY( setEqualsList(QSet<Component*>() << dogsModel << maleDogsModel << femaleDogsModel, components) );
}

void TestUtilities::testFindAtomicDescendantsSome() {
    QList<QObject*> objects = seekDescendants<QObject*>("female", mammals);
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(QSet<QObject*>() << femaleDogs << femaleDogsModel, objects) );
}

void TestUtilities::testFindAtomicDescendantsNone() {
    QList<QObject*> objects = seekDescendants<QObject*>("cats", reptiles);
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testFindJokerFirst() {
    QList<QObject*> objects;
    QSet<QObject*> females;
    females << femaleDogs << femaleDogsModel;

    objects = seekDescendants<QObject*>("*/female", 0);
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(females, objects) );

    objects = seekDescendants<QObject*>("*/*/female", 0);
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(females, objects) );

    objects = seekDescendants<QObject*>("*/animals", 0);
    QCOMPARE(objects.size(), 0);

    objects = seekDescendants<QObject*>("*/reptiles", 0);
    QCOMPARE(objects.size(), 1);
    QCOMPARE(objects[0], reptiles);

    objects = seekDescendants<QObject*>("*/*reptiles", 0);
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testFindJokerInside() {
    QList<QObject*> objects;
    QSet<QObject*> females;
    females << femaleDogs << femaleDogsModel;

    objects = seekDescendants<QObject*>("mammals/*/female", 0);
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(females, objects) );

    objects = seekDescendants<QObject*>("animals/*/*/female", 0);
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(females, objects) );

    objects = seekDescendants<QObject*>("animals/*/*/*/female", 0);
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testFindJokerLast() {
    QList<QObject*> objects;
    objects = seekDescendants<QObject*>("animals/reptiles/*", 0);
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(QSet<QObject*>() << snakes << lizards, objects) );

    objects = seekDescendants<QObject*>("mammals/*/*", 0);
    QCOMPARE(objects.size(), 4);

    objects = seekDescendants<QObject*>("dogs/*/*", 0);
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
        models = seekDescendants<Model*>("/dogs/female", 0);
        QCOMPARE(models.size(), 1);
        QCOMPARE(models[0], femaleDogsModel);

        models = seekDescendants<Model*>("/dogs/female/", 0);
        QCOMPARE(models.size(), 1);
        QCOMPARE(models[0], femaleDogsModel);

        models = seekDescendants<Model*>("dogs/female/", 0);
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
        objects = seekDescendants<QObject*>("///dogs/female", 0);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        objects = seekDescendants<QObject*>("dogs//female", 0);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        objects = seekDescendants<QObject*>("dogs/female//", 0);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
}

void TestUtilities::testFindEmpty() {
    QList<QObject*> objects;
    objects = seekDescendants<QObject*>("", 0);
    QCOMPARE(objects.size(), 0);

    objects = seekDescendants<QObject*>("/", 0);
    QCOMPARE(objects.size(), 0);
}

void TestUtilities::testFindChildrenFromRoot() {
    QList<QObject*> objects;
    objects = UniSim::seekChildren<QObject*>("*", 0);
    QCOMPARE(objects.size(), 2);
    QVERIFY( setEqualsList(QSet<QObject*>() << mammals << reptiles, objects) );
    objects = UniSim::seekChildren<QObject*>("mammals", 0);
    QCOMPARE(objects.size(), 1);
    QCOMPARE(objects[0], mammals);

    objects = UniSim::seekChildren<QObject*>("insects", 0);
    QCOMPARE(objects.size(), 0);

    QList<QString*> strings = UniSim::seekChildren<QString*>("mammals", 0);
    QCOMPARE(strings.size(), 0);
}

void TestUtilities::testFindChildrenFromParent() {
    QList<QObject*> objects;

    objects = UniSim::seekChildren<QObject*>("*", mammals);
    QCOMPARE(objects.size(), 4);
    QVERIFY( setEqualsList(QSet<QObject*>() << mice << elephants << dogs << dogsModel, objects) );

    QList<Model*> models = UniSim::seekChildren<Model*>("*", mammals);
    QCOMPARE(models.size(), 1);
    QCOMPARE(models[0], dogsModel);

    objects = UniSim::seekChildren<QObject*>("mice", mammals);
    QCOMPARE(objects.size(), 1);
    QCOMPARE(objects[0], mice);

    objects = UniSim::seekChildren<QObject*>("bats", mammals);
    QCOMPARE(objects.size(), 0);

    QList<QString*> strings = UniSim::seekChildren<QString*>("mice", 0);
    QCOMPARE(strings.size(), 0);
}

void TestUtilities::testFindChildFromRoot() {
    QObject* object;
    bool excepted;

    excepted = false;
    try {
        object = UniSim::seekOneChild<QObject*>("*", 0);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        object = UniSim::seekOneChild<QObject*>("mammals", 0);
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
        object = UniSim::seekOneChild<QObject*>("*", mammals);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);

    excepted = false;
    try {
        object = UniSim::seekOneChild<QObject*>("mice", mammals);
        QCOMPARE(object, mice);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(!excepted);
}
