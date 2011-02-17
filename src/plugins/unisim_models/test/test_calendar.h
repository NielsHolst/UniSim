#ifndef TEST_UNISIM_CALENDAR_H
#define TEST_UNISIM_CALENDAR_H

#include <QDate>
#include <QList>
#include <usbase/test/autotest.h>

namespace UniSim {
    class Model;
}

class TestCalendar : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void init();
    void cleanup();

    void testSolarElevation();
    void testDayLength();
    void testDaySteps();
    void testHourSteps();

private:
    QDate finalCalendarDate();

    struct Location{
        Location(QString nam, double lat, double lon) :
            name(nam), latitude(lat), longitude(lon) {}
        QString name;
        double latitude, longitude;
    };

    QList<Location> locations;
    QList<QDate> dates;
    QList<double> hours;

    double solarNoonDiff[4][4];
    QTime dayLength[4][4];
    double solarElev[4][4][3];

    UniSim::Model *calendar;
};

DECLARE_TEST(TestCalendar)


#endif
