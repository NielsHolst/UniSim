#include <usbase/test_num.h>
#include "test_general.h"
#include "../general.h"

using namespace vg;

void TestGeneral::testCo2() {
    double absCo2 = absFromPpmCo2(25, 380);
    QVERIFY(TestNum::eq(absCo2, 0.683607e-3));
    double ppm = ppmFromAbsCo2(25, absCo2);
    QVERIFY(TestNum::eq(ppm, 380.));
}

