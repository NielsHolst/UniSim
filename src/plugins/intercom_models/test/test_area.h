#ifndef UNISIM_TEST_AREA_H
#define UNISIM_TEST_AREA_H

#include <QList>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
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

    void testAtHeight();
    void testAboveHeight();
    void testCalcEffectiveAreaAbove();
    void testCalcAbsorptionInShade();
    void testCalcPhotosynthesisInShade();
    void testCalcPhotosynthesisInSun();
    void testCalcPhotosynthesisInTotal();
private:
    void reset();

    UniSim::Simulation *simulation;
    UniSim::Model *calendar, *weather, *height, *lightUseEfficiency, *assimilationMax;
    intercom::Area *leafArea;
};

DECLARE_TEST(TestArea)


#endif
