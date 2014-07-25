#include <usbase/model.h>
#include <usbase/test_num.h>
#include <usengine/simulation.h>
#include "create.h"
#include "test_construction_geometry.h"

using namespace UniSim;

void TestConstructionGeometry::testConstructionGeometry() {
    Simulation *sim = vg::create::simulation("test_construction_geometry.xml");
    sim->execute();
    Model *gh = sim->seekOne<Model*>("gh");
    double roofHeight = 10./sqrt(3.),
           roofWidth = 2*roofHeight;
    QVERIFY(TestNum::eq(gh->pullValue<double>("width"), 60.));
    QVERIFY(TestNum::eq(gh->pullValue<double>("length"), 50.));
    QVERIFY(TestNum::eq(gh->pullValue<double>("groundArea"), 3000.));
    QVERIFY(TestNum::eq(gh->pullValue<double>("sideWallsArea"), 100*4.));
    QVERIFY(TestNum::eq(gh->pullValue<double>("endWallsArea"), 120*4.));
    QVERIFY(TestNum::eq(gh->pullValue<double>("gablesArea"), 3*2*roofHeight*10));
    QVERIFY(TestNum::eq(gh->pullValue<double>("roofArea"), 3*2*roofWidth*50));
    QVERIFY(TestNum::eq(gh->pullValue<double>("coverArea"), (100+120)*4 + 6*(roofHeight*10 + roofWidth*50)));
    QVERIFY(TestNum::eq(gh->pullValue<double>("volumeBelowRoof"), 3000*4.));
    QVERIFY(TestNum::eq(gh->pullValue<double>("volumeRoof"), 3*roofHeight*10*50));
    QVERIFY(TestNum::eq(gh->pullValue<double>("volumeTotal"), 3000*4 + 3*roofHeight*10*50));
    QVERIFY(TestNum::eq(gh->pullValue<double>("averageHeight"), (3000*4 + 3*roofHeight*10*50)/3000));
    delete sim;
}
