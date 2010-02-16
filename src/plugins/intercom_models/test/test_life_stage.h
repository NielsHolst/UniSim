#ifndef TEST_LIFE_STAGE_H
#define TEST_LIFE_STAGE_MAKER_H

#include <usbase/test/autotest.h>

class TestLifeStage : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void cleanupTestCase();
	void testUpdate();
	void testState();
};

DECLARE_TEST(TestLifeStage)


#endif
