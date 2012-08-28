#include <iostream>
#include <boost/math/distributions/lognormal.hpp>
#include <boost/math/distributions/normal.hpp>
#include <boost/math/distributions/poisson.hpp>
#include <QVector>
#include <usbase/exception.h>
#include <usbase/model.h>
#include <usbase/parameter.h>
#include <usbase/test_num.h>
#include <usengine/mega_factory.h>
#include "test_random.h"

using namespace UniSim;


const int N = 100;
const int UNIFORM_MIN = 10;
const int UNIFORM_MAX = 30;

struct Expected {
    TestRandom::Data uniform, lognormal, normal, poisson;

    Expected() {
        boost::math::lognormal ln(10., 2.);
        boost::math::normal n(10., 2.);
        boost::math::poisson p(10.);
        uniform.resize(N);
        lognormal.resize(N-1);
        normal.resize(N-1);
        poisson.resize(N-1);
        for (int i = 0; i < N; ++i) {
            double prob = double(i+1)/N;
            uniform[i].second = true;
            uniform[i].first = UNIFORM_MIN + prob*(UNIFORM_MAX-UNIFORM_MIN);
            if (N < i-1) {
                lognormal[i].second = true;
                lognormal[i].first = quantile(n, prob);
                normal[i].second = true;
                normal[i].first = quantile(n, prob);
                poisson[i].second = true;
                poisson[i].first = quantile(p, prob);
            }
        }
    }

} expected;

void TestRandom::testLargestDiff() {
    const double a[] = {1.26, 0.34, 0.70, 1.75, 50.57, 1.55, 0.08, 0.42, 0.50, 3.20,
                        0.15, 0.49, 0.95, 0.24, 1.37, 0.17, 6.98, 0.10, 0.94, 0.38};
    const double b[] = {2.37, 2.16, 14.82, 1.73, 41.04, 0.23, 1.32, 2.91, 39.41, 0.11,
                        27.44, 4.51, 0.51, 4.50, 0.18, 14.68, 4.66, 1.30, 2.06, 1.19};
    Data x, y;
    x.resize(20);
    y.resize(20);
    for (int i = 0; i < 20; ++i) {
        x[i].first = a[i];
        x[i].second = true;
        y[i].first = b[i];
        y[i].second = false;
    }
    double D = largestDiff(x, y);
    QVERIFY(TestNum::eq(D,0.45));
}

void TestRandom::testP() {
    double D = largestDiff(expected.uniform, expected.normal);
    QVERIFY(D > P());

}

void TestRandom::testUniform() {
    Model *model = MegaFactory::create<Model>("RandomUniform", "test", 0);
    setMinMax(model, UNIFORM_MIN, UNIFORM_MAX);
    model->initialize();
    generateObs(model);
    double D = largestDiff(obs, expected.uniform);
    QVERIFY(D < P());
    delete model;
}

void TestRandom::testLognormal() {
    Model *model = MegaFactory::create<Model>("RandomLognormal", "test", 0);
    model->initialize();
    generateObs(model);
    double D = largestDiff(obs, expected.lognormal);
    QVERIFY(D < P());
    delete model;
}

void TestRandom::testNormal() {
    Model *model = MegaFactory::create<Model>("RandomNormal", "test", 0);
    model->initialize();
    generateObs(model);
    double D = largestDiff(obs, expected.normal);
    QVERIFY(D < P());
    delete model;
}

void TestRandom::testPoisson() {
    Model *model = MegaFactory::create<Model>("RandomPoisson", "test", 0);
    model->initialize();
    generateObs(model);
    double D = largestDiff(obs, expected.poisson);
    QVERIFY(D < P());
    delete model;
}

void TestRandom::generateObs(UniSim::Model *model) {
    obs.resize(N);
    for (int i = 0; i < N; ++i) {
        model->deepReset();
        obs[i].first = model->pullValue<double>("value");
        obs[i].second = false;
    }
}

double TestRandom::largestDiff(const TestRandom::Data &x, const TestRandom::Data &y) {
    TestRandom::Data all;
    all << x << y;
    qSort(all);
    double nx = x.size(), ny = y.size();
    double diff(0), Px(0), Py(0), P(0), P0(0);
    for (int i = 0; i < all.size(); ++i) {
        P0 = P;
        if (all.value(i).second)
            P = Px += 1/nx;
        else
            P = Py += 1/ny;
        if (i > 0) {
            double d = fabs(P - P0);
            if (all[i].second != all[i-1].second && d > diff)
                diff = d;
        }
    }
    return diff;
}

double TestRandom::P() {
    return 1.63/sqrt(N);    // alpha = 0.01
}

void TestRandom::testMinMaxOk() {
    Model *model = MegaFactory::create<Model>("RandomNormal", "test", 0);
    model->initialize();
    setMinMax(model, 6, 14);
    try {
        generateObs(model);
        for (int i = 0; i< obs.size(); ++i) {
            QVERIFY(obs.value(i).first>=6 && obs.value(i).first<=14);
        }

    }
    catch (Exception &ex) {
        QString s = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(s));
    }
    delete model;
}

void TestRandom::testMinMaxError() {
    Model *model = MegaFactory::create<Model>("RandomNormal", "test", 0);
    model->initialize();
    setMinMax(model, 0,0.001);
    bool excepted = false;
    try {
        generateObs(model);
    }
    catch (Exception &ex) {
        excepted = true;
    }
    QVERIFY(excepted);
    delete model;
}

void TestRandom::setMinMax(Model *model, double min, double max) {
    typedef Parameter<double> Param;
    Param *minValue, *maxValue;
    minValue = model->seekOneChild<Param*>("minValue");
    minValue->setValue(min);
    maxValue = model->seekOneChild<Param*>("maxValue");
    maxValue->setValue(max);
}
