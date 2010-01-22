#ifndef TEST_MODEL_MAKER_H
#define TEST_MODEL_MAKER_H

#include <usbase/test/autotest.h>

class TestModelMaker : public QObject
{
    Q_OBJECT
private slots:
	void testCreation();
};

DECLARE_TEST(TestModelMaker)


#endif
