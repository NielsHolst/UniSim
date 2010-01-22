#include <iostream>
#include <QList>
#include <usbase/exception.h>
#include <usengine/test/test_global_objects_dll/test_global_objects_dll.h>
#include "test_global_objects.h"

using namespace std;
using namespace UniSim;
	
void TestGlobalObjects::initTestCase() {
    // Create objects
    a = new QObject; a->setObjectName("a");
    b = new QObject; b->setObjectName("b");
    c = new QObject; c->setObjectName("c");
    objects.append(a);
    objects.append(b);
    objects.append(c);

    // Copy to shared memory
    sharedMemory.setKey("TestUniSimSharedMemory");
    sharedMemory.attach();

    QList<QObject*> *pValue = &objects;
    int size = sizeof(objects);
    bool createOk = sharedMemory.create(size);
    QVERIFY(createOk);

    sharedMemory.lock();
    const char *source = (const char *) pValue;
    char *buf  = (char *) sharedMemory.data();

    QList<QObject*> *myObjects;
    myObjects = new (buf) QList<QObject*>;

    memcpy(buf, source, size);

    QCOMPARE(objects.size(), myObjects->size());
    QCOMPARE(myObjects->at(0), a);
    QCOMPARE(myObjects->at(1), b);
    QCOMPARE(myObjects->at(2), c);

    sharedMemory.unlock();

    // Copy to global objects
    globals.setObjects(objects);
}

void TestGlobalObjects::testMemcpyChar() {
    char *value = "abcdef";
    int size = 128;

    const char *source = value;
    char *buf  = new char[size];
    memcpy(buf, source, size);

    char *copy = buf;

    QCOMPARE(value, copy);

    delete[] buf;
}

void TestGlobalObjects::testMemcpyDouble() {
    double value = 123.456;
    double *pValue = &value;
    int size = sizeof(value);

    const char *source = (const char *) pValue;
    char *buf  = new char[size];
    memcpy(buf, source, size);

    double *pCopy = (double*) buf;
    double copy = *pCopy;

    QCOMPARE(value, copy);

    delete[] buf;
}

void TestGlobalObjects::testMemcpyObjects() {
    QList<QObject*> *pValue = &objects;
    int size = sizeof(objects);

    const char *source = (const char *) pValue;
    char *buf  = new char[size];

    memcpy(buf, source, size);

    QList<QObject*> *pCopy = (QList<QObject*> *) buf;

    QCOMPARE(objects.size(), pCopy->size());
    QCOMPARE(pCopy->at(0), a);
    QCOMPARE(pCopy->at(1), b);
    QCOMPARE(pCopy->at(2), c);

    delete[] buf;
}

void TestGlobalObjects::testPlacementNew() {
    QList<QObject*> *pValue = &objects;
    int size = sizeof(objects);

    const char *source = (const char *) pValue;
    char *buf  = new char[size];

    QList<QObject*> *myObjects;
    myObjects = new (buf) QList<QObject*>;

    memcpy(buf, source, size);

    QCOMPARE(objects.size(), myObjects->size());
    QCOMPARE(myObjects->at(0), a);
    QCOMPARE(myObjects->at(1), b);
    QCOMPARE(myObjects->at(2), c);

    delete[] buf;
}

void TestGlobalObjects::TestSharedMemory() {
    sharedMemory.lock();
    int size = sharedMemory.size();

    const char *source = (const char *) sharedMemory.constData();
    char *buf  = new char[size];

    QList<QObject*> *myObjects;
    myObjects = new (buf) QList<QObject*>;

    memcpy(buf, source, size);

    QCOMPARE(objects.size(), myObjects->size());
    QCOMPARE(myObjects->at(0), a);
    QCOMPARE(myObjects->at(1), b);
    QCOMPARE(myObjects->at(2), c);

    sharedMemory.unlock();
    delete[] buf;
}

void TestGlobalObjects::testExeModule() {
    QCOMPARE(globals.find<QObject*>("a"), a);
    QCOMPARE(globals.find<QObject*>("b"), b);
    QCOMPARE(globals.find<QObject*>("c"), c);
}

void TestGlobalObjects::testDllPresence() {
    TestGlobalObjectsDll dll;
    QCOMPARE(dll.hello(), QString("hello"));
}

void TestGlobalObjects::testGlobalsInDll() {
    TestGlobalObjectsDll dll;
    QCOMPARE(dll.find("a"), a);
    QCOMPARE(dll.find("b"), b);
    QCOMPARE(dll.find("c"), c);
}
