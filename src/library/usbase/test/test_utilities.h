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
    void testDecodeSimpleListEmpty();
    void testDecodeSimpleListOne();
    void testDecodeSimpleListMany();
    void testDecodeSimpleListFaulty();

    void testQStringSplit();
    void testSplitAtNamespace();
    void testSplitParentChildExpression();
    void testMissingLeftBracket();
    void testMissingRightBracket();
    void testMissplacedLeftBracket();
    void testMissingChildName();

    void testStringToValueDouble();
    void testStringToValueInt();
    void testStringToValueChar();
    void testStringToValueBool();
    void testStringToValueDate();
    void testStringToValueTime();

    void testStringToMissingValueDouble();
    void testStringToMissingValueInt();
    void testStringToMissingValueChar();
    void testStringToMissingValueBool();
    void testStringToMissingValueDate();
    void testStringToMissingValueTime();

    void testDecodeListFaulty();
    void testDecodeListEmpty();
    void testDecodeListEmptyItems();
    void testDecodeListDepth1();
    void testDecodeListDepth2();
    void testDecodeListOfInts();
    void testDecodeListOfDates();
    void testDecodeListOfStringDoubles();
    void testDecodeListOfStringDoubles2();
    void testDecodeListOfIntDates();

    void testDivBounded();

    void testFindNearestFile();
};

DECLARE_TEST(TestUtilities)

#endif 
