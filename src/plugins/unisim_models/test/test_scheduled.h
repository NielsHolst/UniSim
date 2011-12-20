#ifndef TEST_UNISIM_SCHEDULED_H
#define TEST_UNISIM_SCHEDULED_H

#include <QDir>
#include <QFile>
#include <QVector>
#include <QStringList>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Simulation;
}

class TestScheduled : public QObject
{
    Q_OBJECT
private slots:
    void cleanup();
    void testSchedule();

private:
    void createSimulation(QString filename);
    QString filePath(QString fileName);
    QDir inputFolder();

    UniSim::Simulation *sim;
};

DECLARE_TEST(TestScheduled)


#endif
