#ifndef UNISIM_TEST_REFERENCE_H
#define UNISIM_TEST_REFERENCE_H

#include <QObject>
#include <usbase/test/autotest.h>
	
class TestReference : public QObject
{
    Q_OBJECT
private slots:
    void testSplit();
    void testMissingLeftBracket();
    void testMissingRightBracket();
    void testMissplacedLeftBracket();
    void testMissingName();
};

DECLARE_TEST(TestReference)

#endif 
