#ifndef UNISIM_TEST_STRATA_H
#define UNISIM_TEST_STRATA_H

#include <QObject>
#include <QVector>
#include <usbase/test/autotest.h>

class TestStrata : public QObject
{
    Q_OBJECT
private slots:
    void testAbsoluteInt();
    void testAbsoluteDouble();
    void testAbsoluteDate();
    void testRelativeInt();
    void testRelativeDouble();
    void testFactorInt();
    void testFactorDouble();

private:
    template<class T>
    void comp(const QVector<T> *strata, const QVector<T> *samples)
    {
        for (int i = 0; i < samples->size(); ++i) {
            T low = strata->value(i), high = strata->value(i+1), between = samples->value(i);
            if (low > high) {
                T x = low;
                low = high;
                high = x;
            }
            QVERIFY(low<=between && between<=high);
        }
    }
};

DECLARE_TEST(TestStrata)


#endif
