#include <QVector>
#include "../plot.h"
#include "test_plot.h"

using namespace UniSim;

void TestPlot::testSetIntervals1() {
    QVector<double> x, y;

    y.append(1);
    y.append(1);
    y.append(0);
    y.append(0);
    y.append(1);
    y.append(1);

    x.fill(2,y.size());

    Plot plot;
    plot.x = &x;
    plot.y = &y;
    plot.logy = true;
    plot.setIntervals();

    Plot::Intervals intervals = plot.getIntervals();
    QCOMPARE(intervals.size(), 2);
    QCOMPARE(intervals[0], Plot::Interval(0,1));
    QCOMPARE(intervals[1], Plot::Interval(4,5));
}

void TestPlot::testSetIntervals2() {
    QVector<double> x, y;

    y.append(0);
    y.append(0);
    y.append(1);
    y.append(1);
    y.append(0);
    y.append(0);

    x.fill(2,y.size());

    Plot plot;
    plot.x = &x;
    plot.y = &y;
    plot.logy = true;
    plot.setIntervals();

    Plot::Intervals intervals = plot.getIntervals();
    QCOMPARE(intervals.size(), 1);
    QCOMPARE(intervals[0], Plot::Interval(2,3));
}


void TestPlot::testSetIntervals3() {
    QVector<double> x, y;

    y.append(0);

    x.fill(2,y.size());

    Plot plot;
    plot.x = &x;
    plot.y = &y;
    plot.logy = true;
    plot.setIntervals();

    Plot::Intervals intervals = plot.getIntervals();
    QCOMPARE(intervals.size(), 0);
}

void TestPlot::testSetIntervals4() {
    QVector<double> x, y;

    y.append(1);

    x.fill(2,y.size());

    Plot plot;
    plot.x = &x;
    plot.y = &y;
    plot.logy = true;
    plot.setIntervals();

    Plot::Intervals intervals = plot.getIntervals();
    QCOMPARE(intervals.size(), 1);
    QCOMPARE(intervals[0], Plot::Interval(0,0));
}
