#ifndef UNISIM_OUTPUTS_TEST_PLOT_H
#define UNISIM_OUTPUTS_TEST_PLOT_H

#include <usbase/test/autotest.h>

class TestPlot : public QObject
{
    Q_OBJECT

private slots:
    void testSetIntervals1();
    void testSetIntervals2();
    void testSetIntervals3();
    void testSetIntervals4();
};

DECLARE_TEST(TestPlot)

#endif
