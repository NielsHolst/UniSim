#ifndef TEST_UNISIM_SUM_H
#define TEST_UNISIM_SUM_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
    class Simulation;
}

class TestSum : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();
    void testSum();

private:
    void createSimulation(QString filename);

    UniSim::Simulation *sim;
};

DECLARE_TEST(TestSum)


#endif
