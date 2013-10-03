#ifndef UNISIM_TEST_NAMED_OBJECT_H
#define UNISIM_TEST_NAMED_OBJECT_H

#include <QObject>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Component;
    class Model;
    class NamedObject;
}
	
class TestNamedObject : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void cleanupTestCase();

    void testFindSlashes();
    void testFindExceptions();
    void testFindEmpty();

    void testFindAtomicGloballyAll();
    void testFindAtomicGloballySome();
    void testFindAtomicGloballyNone();
    void testFindAtomicDescendantsAll();
    void testFindAtomicDescendantsSome();
    void testFindAtomicDescendantsNone();
    void testFindJokerFirst();
    void testFindJokerInside();
    void testFindJokerLast();

    void testFindChildrenFromRoot();
    void testFindChildrenFromParent();
    void testFindChildFromRoot();
    void testFindChildFromParent();

    void testSeekChildAndParentNone();
    void testSeekChildAndParentOne();
    void testSeekChildrenAndParentsMany();
    void testSeekChildrenAndParentsJoker();

    void testPeekOne();
    void testPeekOneChild();
    void testPeekOneDescendant();
    void testPeekOneAscendant();

    void testPeekOneNearest();
    void testSeekOneNearest();

    void testPeekParent();
    void testSeekParent();

    void testRelativeSelf();
    void testRelativeSelfBare();
    void testRelativeSibling();
    void testRelativeSiblingBare();
    void testRelativeFromRoot();
    void testRelativeAtRoot();
    void testRelativeAtRootBare();
    void testRelativeBeyondRoot();
    void testRelativeNot();

    void testRelativeSeek();


private:
    UniSim::NamedObject
        *animals, *femaleAnimals, *reptiles, *femaleReptiles, *snakes, *cobra, *lizards,
        *mammals, *mice, *elephants, *femaleElephants, *dogs, *femaleDogs, *maleDogs;
    UniSim::Model
        *dogsModel, *femaleDogsModel, *maleDogsModel;
    int femaleDogsSize, maleDogsSize;
    int femaleDogsWeight, maleDogsWeight;
};

DECLARE_TEST(TestNamedObject)

#endif 
