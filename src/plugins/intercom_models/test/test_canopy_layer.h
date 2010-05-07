#ifndef UNISIM_TEST_CANOPY_LAYER
#define UNISIM_TEST_CANOPY_LAYER

#include <QList>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
    class Simulation;
}

namespace intercom {
    class CanopyLayer;
}

class TestCanopyLayer : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void cleanupTestCase();

    void testCalcEffectiveAreaAbove();
private:
    void reset();

    UniSim::Simulation *simulation;
    UniSim::Model *calendar, *plant;
    QList<intercom::CanopyLayer*> canopyLayers;
};

DECLARE_TEST(TestCanopyLayer)


#endif
