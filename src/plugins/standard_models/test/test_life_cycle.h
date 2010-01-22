#ifndef TEST_LIFE_CYCLE_H
#define TEST_LIFE_CYCLE_MAKER_H

#include <usbase/test/autotest.h>


class TestLifeCycle : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void cleanupTestCase();
	void testUpdate();
};

DECLARE_TEST(TestLifeCycle)


#endif
