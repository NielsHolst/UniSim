#ifndef UNISIM_TEST_MATRIX_H
#define UNISIM_TEST_MATRIX_H

#include <QObject>
#include <usbase/test/autotest.h>


class TestMatrix : public QObject
{
    Q_OBJECT
private slots:
    void testEmpty();
    void test1By1();
    void testNByN();
    void testResize();
};

DECLARE_TEST(TestMatrix)


#endif
