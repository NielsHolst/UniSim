#ifndef UNISIM_TEST_MOMENTS_H
#define UNISIM_TEST_MOMENTS_H

#include <QObject>
#include <usbase/test/autotest.h>


class TestMoments : public QObject
{
    Q_OBJECT
private slots:
	void testCalculations();
};

DECLARE_TEST(TestMoments)


#endif
