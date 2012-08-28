#include <usbase/model.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "../output_plot.h"
#include "test_output_plot.h"

using namespace UniSim;

void TestOutputPlot::testTimeStep1D() {
    QString filePath = TestOutputPlot::filePath("timestep_1d.xml");
    SimulationMaker maker;
    Simulation * sim = 0;
    try {
        sim = maker.parse(filePath);
    }
    catch (Exception &ex) {
        QString msg = "Unexpected exception. " + ex.message();
        QFAIL(qPrintable(msg));
    }

    OutputPlot *plot = sim->seekOneChild<OutputPlot*>("*");
    QVERIFY(plot);
    QList<OutputPlot::TableRecord> records = plot->tableRecords();
    QCOMPARE(records.size(), 1);
    OutputPlot::TableRecord &rec(records[0]);
    QCOMPARE(rec.x.size(), 32);
    QCOMPARE(rec.x[0], 1.);
    QCOMPARE(rec.x[1], 3.);
    QCOMPARE(rec.x[31], (double) QDate(2008,4,1).daysTo(QDate(2008,8,15)));

    QCOMPARE(rec.yList.size(), 4);
    QStringList labels, expectedLabels;
    expectedLabels << "DIGSA" << "ELEIN" << "SETGL" << "SETVI";
    for (int i = 0; i < rec.yList.size(); ++i) {
        labels << rec.yList[i].label;
    }
    QCOMPARE(labels, expectedLabels);

    checkY(rec.yList[0].data, 0, 0.3, 96.6);
    checkY(rec.yList[3].data, 13, 22.3, 99.3);
}

void TestOutputPlot::checkY(QVector<double> &y, double first, double second, double mid) {
    QCOMPARE(y[0], first);
    QCOMPARE(y[1], second);
    QCOMPARE(y[20], mid);
}

QString TestOutputPlot::filePath(QString fileName) const {
    QDir uniSimDir = QDir("../..");
    return uniSimDir.absolutePath() +
            "/src/plugins/unisim/test/input/output_plot/" +
            fileName;
}

