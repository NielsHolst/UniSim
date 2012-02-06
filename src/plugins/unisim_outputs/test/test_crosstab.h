#ifndef UNISIM_OUTPUTS_TEST_CROSSTAB_H
#define UNISIM_OUTPUTS_TEST_CROSSTAB_H
#include <QStringList>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Simulation;
}

class TestCrosstab : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testOutput();
    void testOutputTransposed();

private:
    QString inputFilePath(QString fileName) const;
    QString outputFilePath(QString fileName) const;
    void checkFileExists(QString fileName) const;

    UniSim::Simulation *sim;
    QStringList crops, farms, cropClass, farmClass;
};

DECLARE_TEST(TestCrosstab)

#endif
