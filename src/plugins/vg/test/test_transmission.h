#ifndef TEST_VG_CONSTRUCTION_GEOMETRY_H
#define TEST_VG_CONSTRUCTION_GEOMETRY_H

#include <usbase/model.h>
#include <usbase/test/autotest.h>
#include <usengine/simulation.h>

class TestTransmission : public QObject
{
    Q_OBJECT
private slots:
    void setup(double screenState);

    void testCommonKFullScreen();
    void testCommonKPartScreen();

    void testCommonHazeFullScreen();
    void testCommonHazePartScreen();

    void testCommonDiffuseTransmissionFullScreen();
    void testCommonDiffuseTransmissionPartScreen();

    void init();
    void cleanup();
private:
    UniSim::Simulation *sim;
    UniSim::Model
        *geometry,
        *anyCover,
        *screen,
        *trans,
        *screenControl;
    double
        totalArea,
        roofArea,
        coverU,
        coverHaze,
        coverDiffuseTransmission,
        screenReduction,
        screenLightTransmission,
        f;


};

DECLARE_TEST(TestTransmission)


#endif
