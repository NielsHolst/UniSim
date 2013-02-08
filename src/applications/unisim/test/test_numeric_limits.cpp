#include <cmath>
#include <iostream>
#include <limits>
#include "test_numeric_limits.h"

using std::numeric_limits;

const double INF = numeric_limits<double>::infinity();
const double MAX = numeric_limits<double>::max();
const double EPS = numeric_limits<double>::epsilon();

void TestNumericLimits::testInfinity() {
    QVERIFY(numeric_limits<double>::has_infinity);

    std::cout << "\n" << numeric_limits<double>::has_quiet_NaN << " "
            << numeric_limits<double>::has_signaling_NaN << "\n"
            << 42./0. << " "
            << MAX << " "
            << EPS << " "
            << MAX/EPS << "\n";

    try {
        QVERIFY(42./0. == INF);
        //QVERIFY(MAX/EPS != INF);
        QVERIFY(MAX < INF);
    }
    catch(...) {
        QFAIL("Unexpected exception.");
    }

}
