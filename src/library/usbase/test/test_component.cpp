#include <iostream>
#include "../component.h"
#include "test_component.h"

using namespace UniSim;

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

void TestComponent::initTestCase() {
    animals = create<Component>("animals",0);

    reptiles = create<Component>("reptiles", animals);
    snakes = create<Component>("snakes", reptiles);
    cobra = create<Component>("cobra", snakes);
    lizards = create<Component>("lizards", reptiles);
    mammals = create<Component>("mammals", animals);
    mice[0] = create<Component>("mice", mammals);
    mice[1] = create<Component>("mice", mammals);
    elephants = create<Component>("elephants", mammals);

    dogs = create<Component>("dogs", mammals);
    femaleDogs = create<Component>("female", dogs);
    maleDogs = create<Component>("male", dogs);
}

void TestComponent::cleanupTestCase() {
    delete animals;
}

void TestComponent::testSeekOne() {
    try {
        Component *cob = reptiles->seekOneDescendant<Component*>("cobra");
        QCOMPARE(cob, cobra);
    }
    catch (Exception &ex) {
        QFAIL("Component not found");
    }
}

void TestComponent::testSeekMany() {
    try {
        QList<Component*> dogList = mammals->seekDescendants<Component*>("dogs/*");
        QSet<Component*> dogSet;
        dogSet << femaleDogs << maleDogs;
        QVERIFY(setEqualsList(dogSet, dogList));
    }
    catch (Exception &ex) {
        QFAIL("Components do not match");
    }
}

void TestComponent::testSeekOneChild() {
    Component* liz = reptiles->seekOneChild<Component*>("lizards");
    QCOMPARE(liz, lizards);

    try {
        reptiles->seekOneChild<Component*>("dogs");
        QFAIL("No such child. Exception expected");
    }
    catch (Exception &ex) {
    }

    try {
        dogs->seekOneChild<Component*>("*");
        QFAIL("More than one such child. Exception expected");
    }
    catch (Exception &ex) {
    }
}

void TestComponent::testSeekChildren() {
    try {
        QList<Component*> dogList = dogs->seekChildren<Component*>("*");
        QSet<Component*> dogSet;
        dogSet << femaleDogs << maleDogs;
        QVERIFY(setEqualsList(dogSet, dogList));
    }
    catch (Exception &ex) {
        QFAIL("Components do not match");
    }
}

void TestComponent::testSeekOneSibling() {
    try {
        Component *liz = snakes->seekOneSibling<Component*>("lizards");
        QCOMPARE(liz, lizards);
    }
    catch (Exception &ex) {
        QFAIL("Component not found");
    }

    try {
        dogs->seekOneSibling<Component*>("reptiles");
        QFAIL("No such sibling. Exception expected");
    }
    catch (Exception &ex) {
    }

    try {
        elephants->seekOneSibling<Component*>("mice");
        QFAIL("More than one such sibling. Exception expected");
    }
    catch (Exception &ex) {
    }
}

void TestComponent::testSeekOneSiblingNotMe() {
    try {
        snakes->seekOneSibling<Component*>("snakes");
        QFAIL("No such sibling. Exception expected");
    }
    catch (Exception &ex) {
    }
}

void TestComponent::testSeekSiblings() {
    try {
        QList<Component*> miceList = elephants->seekSiblings<Component*>("mice");
        QSet<Component*> miceSet;
        miceSet << mice[0] << mice[1];
        QVERIFY(setEqualsList(miceSet, miceList));
    }
    catch (Exception &ex) {
        QFAIL("Siblings not found");
    }
}

void TestComponent::testSeekSiblingsNotMe() {
    QList<Component*> miceList;
    try {
        miceList = elephants->seekSiblings<Component*>("mice");
        QSet<Component*> miceSet;
        miceSet << mice[0] << mice[1];
        QVERIFY(setEqualsList(miceSet, miceList));
    }
    catch (Exception &ex) {
        QFAIL("Siblings not found");
    }
}

void TestComponent::testSeekPrecedingSibling() {
    Component *one = mice[1]->seekPrecedingSibling<Component*>("*");
    QCOMPARE(one, mice[0]);
    try {
        mice[0]->seekPrecedingSibling<Component*>("*");
        QFAIL("Exception expected");
    }
    catch (Exception &ex) {
    }
}


void TestComponent::testSeekFollowingSibling() {
    Component *one = elephants->seekFollowingSibling<Component*>("*");
    QCOMPARE(one, dogs);
    try {
        dogs->seekFollowingSibling<Component*>("*");
        QFAIL("Exception expected");
    }
    catch (Exception &ex) {
    }
}


void TestComponent::testSeekOneDescendant() {
    try {
        Component *liz = reptiles->seekOneDescendant<Component*>("lizards");
        QCOMPARE(liz, lizards);
    }
    catch (Exception &ex) {
        QFAIL("Component not found");
    }

    try {
        dogs->seekOneDescendant<Component*>("reptiles");
        QFAIL("No such descendant. Exception expected");
    }
    catch (Exception &ex) {
    }

    try {
        mammals->seekOneDescendant<Component*>("mice");
        QFAIL("More than one such descendant. Exception expected");
    }
    catch (Exception &ex) {
    }
}

void TestComponent::testSeekDescendants() {
    try {
        QList<Component*> miceList = animals->seekDescendants<Component*>("mice");
        QSet<Component*> miceSet;
        miceSet << mice[0] << mice[1];
        QVERIFY(setEqualsList(miceSet, miceList));
    }
    catch (Exception &ex) {
        QFAIL("Descendants not found");
    }
}

void TestComponent::testSeekOneAscendant() {
    try {
        Component *rep = lizards->seekOneAscendant<Component*>("reptiles");
        QCOMPARE(rep, reptiles);
    }
    catch (Exception &ex) {
        QFAIL("Component not found");
    }

    try {
        dogs->seekOneAscendant<Component*>("reptiles");
        QFAIL("No such ascendant. Exception expected");
    }
    catch (Exception &ex) {
    }

    try {
        maleDogs->seekOneAscendant<Component*>("*");
        QFAIL("More than one such ascendant. Exception expected");
    }
    catch (Exception &ex) {
    }
}

void TestComponent::testSeekOneAscendantWithJoker() {
    try {
        Component *dogs_ = maleDogs->seekOneAscendant<Component*>("mammals/*");
        QCOMPARE(dogs_, dogs);
    }
    catch (Exception &ex) {
        QFAIL("Component not found");
    }
}
