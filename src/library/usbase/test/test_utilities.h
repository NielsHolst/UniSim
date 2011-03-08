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

    void testDecodeSimpleListEmpty();
    void testDecodeSimpleListOne();
    void testDecodeSimpleListMany();
    void testDecodeSimpleListFaulty();

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

    void testSplitParentChildExpression();
    void testMissingLeftBracket();
    void testMissingRightBracket();
    void testMissplacedLeftBracket();
    void testMissingChildName();

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

    void testStringToValueChar();
    void testStringToValueTime();

    void testDecodeListFaulty();
    void testDecodeListEmpty();
    void testDecodeListEmptyItems();
    void testDecodeListDepth1();
    void testDecodeListDepth2();
    void testDecodeListOfInts();
    void testDecodeListOfDates();
    void testDecodeListOfStringDoubles();
    void testDecodeListOfIntDates();

private:
    QObject
        *animals, *femaleAnimals, *reptiles, *femaleReptiles, *snakes, *cobra, *lizards,
        *mammals, *mice, *elephants, *femaleElephants, *dogs, *femaleDogs, *maleDogs;
    UniSim::Model
        *dogsModel, *femaleDogsModel, *maleDogsModel;
    int femaleDogsSize, maleDogsSize;
    int femaleDogsWeight, maleDogsWeight;
};

DECLARE_TEST(TestUtilities)

#endif 
