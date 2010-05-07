#ifndef UNISIM_TEST_PLANT_H
#define UNISIM_TEST_PLANT_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
    class Simulation;
}

namespace intercom {
    class Plant;
}

class TestPlant : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void cleanupTestCase();

    void testWeightedAreaAboveLayer();
private:
    void reset();

    UniSim::Simulation *simulation;
    UniSim::Model *calendar, *height;
    intercom::Plant *plant;
};

DECLARE_TEST(TestPlant)


#endif
