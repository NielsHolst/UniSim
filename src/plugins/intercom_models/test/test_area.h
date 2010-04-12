#ifndef UNISIM_TEST_AREA_H
#define UNISIM_TEST_AREA_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Simulation;
}

namespace intercom {
    class Area;
}

class TestArea : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void cleanupTestCase();
    void testWeightedAreaAboveLayer();
    void testLightUseInShade();
    void testLightUseInSun();
    void testLightUseTotal();
private:
    UniSim::Simulation *simulation;
    intercom::Area *area;
};

DECLARE_TEST(TestArea)


#endif
