#ifndef UNISIM_OUTPUTS_TEST_TABLE_H
#define UNISIM_OUTPUTS_TEST_TABLE_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Simulation;
    class DataGrid;
}

class TestTable : public QObject
{
    Q_OBJECT

private slots:
    void test3Levels();
private:
    QString filePath(QString fileName) const;
    UniSim::DataGrid* openOutput(QString fileName) const;
    UniSim::Simulation *_simulation;
};

DECLARE_TEST(TestTable)

#endif
