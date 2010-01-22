#ifndef TEST_FREQUENCY_DISTRIBUTION_H
#define TEST_FREQUENCY_DISTRIBUTION_H

#include <QObject>
#include <usbase/test/autotest.h>


class TestFrequencyDistribution : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void cleanupTestCase();
	void testCalculations();
};

DECLARE_TEST(TestFrequencyDistribution)


#endif
