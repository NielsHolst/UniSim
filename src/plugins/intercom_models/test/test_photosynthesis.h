#ifndef UNISIM_TEST_PHOTOSYNTHESIS
#define UNISIM_TEST_PHOTOSYNTHESIS

#include <QList>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
    class Simulation;
}

namespace intercom {
    class Photosynthesis;
    class Plant;
    class TimeSlice;
}

class TestPhotosynthesis : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void cleanupTestCase();
    void testCalculate();

private:
    void reset();

    UniSim::Simulation *simulation;
    intercom::Photosynthesis *photosynthesis;
    QList<intercom::Plant*> plants;
    QList<intercom::TimeSlice*> timeSlices;
};

DECLARE_TEST(TestPhotosynthesis)


#endif
