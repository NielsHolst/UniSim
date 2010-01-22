#ifndef TEST_GLOBAL_OBJECTS_H
#define TEST_GLOBAL_OBJECTS_H

#include <QSharedMemory>
#include <usbase/test/autotest.h>
#include <usengine/global_objects.h>


class TestGlobalObjects : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void testMemcpyChar();
    void testMemcpyDouble();
    void testMemcpyObjects();
    void testPlacementNew();
    void TestSharedMemory();
    void testExeModule();
    void testDllPresence();
    void testGlobalsInDll();
private:
    UniSim::GlobalObjects globals;
    QSharedMemory sharedMemory;
    QList<QObject*> objects;
    QObject *a, *b, *c;
};

DECLARE_TEST(TestGlobalObjects)

#endif
