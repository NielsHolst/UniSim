#include "../photosynthetic_rate.h"
#include "test_photosynthetic_rate.h"

using namespace intercom;

void TestPhotosyntheticRate::testConstructors() {
    PhotosyntheticRate a, b(7., 13.);
    QCOMPARE(a.absorption(), 0.);
    QCOMPARE(a.assimilation(), 0.);
    QCOMPARE(b.absorption(), 7.);
    QCOMPARE(b.assimilation(), 13.);

}

void TestPhotosyntheticRate::testOperations() {
    PhotosyntheticRate a(2., 8.), b(7., 13.);
    a += b;
    QCOMPARE(a.absorption(), 9.);
    QCOMPARE(a.assimilation(), 21.);

    a *= 3.;
    QCOMPARE(a.absorption(), 27.);
    QCOMPARE(a.assimilation(), 63.);
}
