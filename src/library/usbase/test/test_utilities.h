#ifndef UNISIM_TEST_UTILITIES_H
#define UNISIM_TEST_UTILITIES_H

#include <QObject>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Component;
    class Model;
}
	
class TestUtilities : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();

    void testSplitAtNamespace();

    void testFindAtomicGloballyAll();
    void testFindAtomicGloballySome();
    void testFindAtomicGloballyNone();
    void testFindAtomicDescendantsAll();
    void testFindAtomicDescendantsSome();
    void testFindAtomicDescendantsNone();

    void testFindJokerFirst();
    void testFindJokerInside();
    void testFindJokerLast();

    void testQStringSplit();
    void testFindSlashes();
    void testFindExceptions();
    void testFindEmpty();

    void testFindChildrenFromRoot();
    void testFindChildrenFromParent();
    void testFindChildFromRoot();
    void testFindChildFromParent();

private:
    QObject
        *animals, *reptiles, *snakes, *cobra, *lizards,
        *mammals, *mice, *elephants, *dogs, *femaleDogs, *maleDogs;
    UniSim::Model
        *dogsModel, *femaleDogsModel, *maleDogsModel;
};

DECLARE_TEST(TestUtilities)

#endif 
