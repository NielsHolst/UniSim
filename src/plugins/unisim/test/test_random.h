#ifndef TEST_UNISIM_RANDOM_H
#define TEST_UNISIM_RANDOM_H

#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
}

class TestRandom : public QObject
{
    Q_OBJECT
public:
    typedef QVector<QPair<double, bool> > Data;

private slots:
    void testLargestDiff();
    void testP();
    void testUniform();
    void testLognormal();
    void testNormal();
    void testPoisson();
    void testMinMaxOk();
    void testMinMaxError();

private:
    Data obs;
    void generateObs(UniSim::Model *model);
    double largestDiff(const TestRandom::Data &x, const TestRandom::Data &y);
    double P();
    void setMinMax(UniSim::Model *model, double min, double max);
};

DECLARE_TEST(TestRandom)


#endif
