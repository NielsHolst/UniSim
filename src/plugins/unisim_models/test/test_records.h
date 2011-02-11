#ifndef TEST_UNISIM_CALENDAR_H
#define TEST_UNISIM_CALENDAR_H

#include <QDate>
#include <QList>
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
    void testColumnLabels();

private:
    void createSimulation(QString filename);
    UniSim::Simulation *sim;
};

DECLARE_TEST(TestRecords)


#endif
