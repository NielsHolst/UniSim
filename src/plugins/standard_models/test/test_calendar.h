#ifndef TEST_UNISIM_CALENDAR_H
#define TEST_UNISIM_CALENDAR_H

#include <QDate>
#include <QList>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
    class Simulation;
}

class TestCalendar : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void cleanupTestCase();
    void cleanup();

    void testSolarElevation();
    void testDayLength();

    void testFirstDateNoneNoFollower();
    void testFirstDateNoneWithFollowerWithFirstDate();
    void testFirstDateNoneWithFollowerWithoutFirstDate();

    void testFirstDatePresentNoFollower();
    void testFirstDatePresentFollowerWithout();
    void testFirstDatePresentFollowerBefore();
    void testFirstDatePresentFollowerSame();
    void testFirstDatePresentFollowerAfter();

    void testFirstDateFollowersConflicting();
    void testFirstDateFollowersNotConflicting();

private:
    void createSimulation(QString filename, int numFollowers);
    QDate finalCalendarDate();

    struct Location{
        Location(QString nam, double lat, double lon) :
            name(nam), latitude(lat), longitude(lon) {}
        QString name;
        double latitude, longitude;
    };

    UniSim::Simulation *sim;
    UniSim::Model *calendar, *weather;

    QList<Location> locations;
    QList<int> days;
    QList<double> hours;

    double solarNoonDiff[4][4];
    QTime dayLength[4][4];
    double solarElev[4][4][3];
};

DECLARE_TEST(TestCalendar)


#endif
