#ifndef UNISIM_OUTPUTS_TEST_CROSSTAB_H
#define UNISIM_OUTPUTS_TEST_CROSSTAB_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Simulation;
}

class TestCrosstab : public QObject
{
    Q_OBJECT

private slots:
    void test3Levels();
private:
    QString filePath(QString fileName) const;
    UniSim::Simulation *_simulation;
};

DECLARE_TEST(TestCrosstab)

#endif
