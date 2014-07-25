#ifndef TEST_UNISIM_RECORDS_H
#define TEST_UNISIM_RECORDS_H

#include <QFile>
#include <QVector>
#include <QStringList>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
    class Simulation;
}

class TestRecords : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();
    void testCalendarDateIsFirst();
    void testCalendarDateIsInside();
    void testTwiceOpened();
private:
    void createSimulation(QString filename);
    void readLineItems();
    void readData(QVector<double> *data);
    void openOutputFile(QString fileName);
    bool compareFiles(QString fileName1, QString fileName2);

    UniSim::Simulation *sim;
    QFile outputFile;
    QStringList lineItems;
};

DECLARE_TEST(TestRecords)


#endif
