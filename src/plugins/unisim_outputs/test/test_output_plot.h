#ifndef UNISIM_OUTPUTS_TEST_OUTPUT_PLOT_H
#define UNISIM_OUTPUTS_TEST_OUTPUT_PLOT_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Simulation;
}

class TestOutputPlot : public QObject
{
    Q_OBJECT

private slots:
    void testTimeStep1D();
private:
    void checkY(QVector<double> &y, double, double, double);
    QString filePath(QString fileName) const;
    UniSim::Simulation *_simulation;
};

DECLARE_TEST(TestOutputPlot)

#endif
