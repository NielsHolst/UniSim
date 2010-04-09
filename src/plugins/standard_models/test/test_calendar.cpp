#include <iostream>
#include <QDir>
#include <usbase/file_locations.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "../anonymous_model.h"
#include "../calendar.h"
#include "../../standard_integrators/simple_integrator.h"
#include "test_calendar.h"

using namespace UniSim;

const QDate WEATHER_DATE = QDate(2009, 9, 1);

namespace local {

    QDate finalDate(QDate firstDate) {
        const int SIM_DURATION = 20;
        return firstDate.addDays(SIM_DURATION-1);
    }

    QString lastOutput() {
        QDir dir = FileLocations::location(FileLocations::Output);
        QString filePath = dir.absolutePath() + "/test_calendar.prn";
        QFile file (filePath);
        bool ok = file.open(QIODevice::ReadOnly | QIODevice::Text);
        Q_ASSERT(ok);

        QString line;
        while (!file.atEnd()) {
            QString nextLine = QString(file.readLine()).trimmed();
            if (!nextLine.isEmpty())
                line = nextLine;
        }
        file.close();
        return line;
    }

    QDate finalWeatherDate() {
        QStringList items = lastOutput().split("\t");
        Q_ASSERT(items.size() == 7);
        int day = items[1].toInt();
        int month = items[2].toInt();
        int year = (int) items[3].toDouble();
        return QDate(year, month, day);
    }
}

QDate TestCalendar::finalCalendarDate() {
    int day = (int) calendar->state("day");
    int month = (int) calendar->state("month");
    int year = (int) calendar->state("year");
    return QDate(year, month, day);
}

/*
NOAA Solar Position Calculator
[http://www.srrb.noaa.gov/highlights/sunrise/azel.html]

Boston:			+42 32" Lat +71 03" Lon (+42.533, +71.050)
Buenos Aires:	-34 36" Lat +58 27" Lon (-34.600, +58.450)
Cape Town: 		-33 55" Lat -18 22" Lon (-33.917, -18.367)
Helsinki:		+60 10"	Lat -24 58" Lon (+60.167, -24.967)

Solar noon
11:53:40
11:44:12
11:50:11
11:29:57
*/

void TestCalendar::initTestCase() {
    // Check locations in all four quadrants:
    locations.append(Location("Boston",         +42.533, -71.050));
    locations.append(Location("Buenos Aires",   -34.600, -58.450));
    locations.append(Location("Cape Town",      -33.917, +18.367));
    locations.append(Location("Helsinki",       +60.167, +24.967));

    // Check dates of all seasons
    days.append(QDate(2010, 1,15).dayOfYear());
    days.append(QDate(2010, 4,15).dayOfYear());
    days.append(QDate(2010, 7,15).dayOfYear());
    days.append(QDate(2010,10,15).dayOfYear());

    // Check night, morning and afternoon
    hours.append(1.);
    hours.append(9.);
    hours.append(16.);

    QTime noon(12,0,0);
    solarNoonDiff[0][0] = noon.secsTo(QTime(11,53,40))/60./60.;
    solarNoonDiff[0][1] = noon.secsTo(QTime(11,44,12))/60./60.;
    solarNoonDiff[0][2] = noon.secsTo(QTime(11,50,11))/60./60.;
    solarNoonDiff[0][3] = noon.secsTo(QTime(11,29,57))/60./60.;

    solarNoonDiff[1][0] = noon.secsTo(QTime(13,06,16))/60./60.;
    solarNoonDiff[1][1] = noon.secsTo(QTime(12,53,49))/60./60.;
    solarNoonDiff[1][2] = noon.secsTo(QTime(12,59,47))/60./60.;
    solarNoonDiff[1][3] = noon.secsTo(QTime(12,39,33))/60./60.;

    solarNoonDiff[2][0] = noon.secsTo(QTime(12,55,54))/60./60.;
    solarNoonDiff[2][1] = noon.secsTo(QTime(12,46,35))/60./60.;
    solarNoonDiff[2][2] = noon.secsTo(QTime(12,52,29))/60./60.;
    solarNoonDiff[2][3] = noon.secsTo(QTime(12,32,19))/60./60.;

    solarNoonDiff[3][0] = noon.secsTo(QTime(12,29,30))/60./60.;
    solarNoonDiff[3][1] = noon.secsTo(QTime(12,20,11))/60./60.;
    solarNoonDiff[3][2] = noon.secsTo(QTime(12,26,05))/60./60.;
    solarNoonDiff[3][3] = noon.secsTo(QTime(12,05,55))/60./60.;

    dayLength[0][0] = QTime( 9, 26,15);
    dayLength[0][1] = QTime(13, 22, 16);
    dayLength[0][2] = QTime(14, 58,14);
    dayLength[0][3] = QTime(11,  5, 55);
    dayLength[1][0] = QTime(14, 11,  8);
    dayLength[1][1] = QTime(11, 13, 24);
    dayLength[1][2] = QTime(10,  4, 21);
    dayLength[1][3] = QTime(12, 55, 42);
    dayLength[2][0] = QTime(14,  9, 24);
    dayLength[2][1] = QTime(11, 14, 38);
    dayLength[2][2] = QTime(10,  6,  4);
    dayLength[2][3] = QTime(12, 54, 29);
    dayLength[3][0] = QTime( 6, 40, 46);
    dayLength[3][1] = QTime(14, 34, 27);
    dayLength[3][2] = QTime(18,  7, 24);
    dayLength[3][3] = QTime(10, 12, 14);

    solarElev[0][0][0] = 0;
    solarElev[0][0][1] = 15.05;
    solarElev[0][0][2] = 5.14;
    solarElev[0][1][0] = 0;
    solarElev[0][1][1] = 41.65;
    solarElev[0][1][2] = 25.88;
    solarElev[0][2][0] = 0;
    solarElev[0][2][1] = 48.89;
    solarElev[0][2][2] = 34.38;
    solarElev[0][3][0] = 0;
    solarElev[0][3][1] = 28.64;
    solarElev[0][3][2] = 10.29;

    solarElev[1][0][0] = 0;
    solarElev[1][0][1] = 35.40;
    solarElev[1][0][2] = 49.02;
    solarElev[1][1][0] = 0;
    solarElev[1][1][1] = 19.15;
    solarElev[1][1][2] = 27.39;
    solarElev[1][2][0] = 0;
    solarElev[1][2][1] = 10.19;
    solarElev[1][2][2] = 19.53;
    solarElev[1][3][0] = 0;
    solarElev[1][3][1] = 33.58;
    solarElev[1][3][2] = 37.42;

    solarElev[2][0][0] = 0;
    solarElev[2][0][1] = 36.90;
    solarElev[2][0][2] = 47.61;
    solarElev[2][1][0] = 0;
    solarElev[2][1][1] = 20.83;
    solarElev[2][1][2] = 26.62;
    solarElev[2][2][0] = 0;
    solarElev[2][2][1] = 11.78;
    solarElev[2][2][2] = 18.92;
    solarElev[2][3][0] = 0;
    solarElev[2][3][1] = 35.14;
    solarElev[2][3][2] = 36.14;

    solarElev[3][0][0] = 0;
    solarElev[3][0][1] = 0;
    solarElev[3][0][2] = 0;
    solarElev[3][1][0] = 0;
    solarElev[3][1][1] = 27.53;
    solarElev[3][1][2] = 25.49;
    solarElev[3][2][0] = 0;
    solarElev[3][2][1] = 37.34;
    solarElev[3][2][2] = 36.40;
    solarElev[3][3][0] = 0;
    solarElev[3][3][1] = 12.22;
    solarElev[3][3][2] = 7.41;
}

void TestCalendar::cleanupTestCase() {
}

void TestCalendar::cleanup() {
    delete sim;
    sim = 0;
}

void TestCalendar::createSimulation(QString fileName, int numFollowers) {
    QDir dir = FileLocations::location(FileLocations::Plugins);
    QString filePath = dir.absolutePath() + "/standard_models/test/" + fileName;

    SimulationMaker maker;
    sim = maker.parse(filePath);

    Identifiers sequence;
    sequence << "calendar";
    if (numFollowers >=1)
        sequence << "weather";
    if (numFollowers ==2)
        sequence << "weather2";
    sim->initialize(sequence);

    calendar = seekOneDescendant<Model*>("calendar", 0);
    try {
        weather = seekOneDescendant<Model*>("weather", 0);
    }
    catch (Exception &ex) {
        weather = 0;
    }
}

void TestCalendar::testSolarElevation() {
    createSimulation("test_calendar_with_follower.xml", 1);

    calendar->changeParameter("firstDate", QDate(2010, 1, 1));
    calendar->deepReset();

    Calendar *cal = dynamic_cast<Calendar*>(calendar);

    for (int lo = 0; lo < locations.size(); ++lo) {
        calendar->changeParameter("latitude", locations[lo].latitude);
        for (int da = 0; da < days.size(); ++da) {
            while (fabs(calendar->state("dayOfYear") - days[da])> 1e-6) {
                calendar->update();
            }
            for (int ho = 0; ho < hours.size(); ++ho) {
                double astroHour = hours[ho] - solarNoonDiff[lo][da];
                double sinb = cal->sinb(astroHour);
                double estSolarElev = asin(sinb)/PI*180.;
                QVERIFY(fabs(estSolarElev - solarElev[lo][da][ho]) < 1.);
            }
        }
    }
}

void TestCalendar::testDayLength() {
    createSimulation("test_calendar_with_follower.xml", 1);

    calendar->changeParameter("firstDate", QDate(2010, 1, 1));
    calendar->deepReset();

    for (int lo = 0; lo < locations.size(); ++lo) {
        calendar->changeParameter("latitude", locations[lo].latitude);
        for (int da = 0; da < days.size(); ++da) {
            while (fabs(calendar->state("dayOfYear") - days[da])> 1e-6) {
                calendar->update();
            }
        double trueDayLength = QTime(0,0,0).secsTo(dayLength[lo][da])/60./60.;
        double estDayLength = calendar->state("dayLength");
        QVERIFY(fabs(estDayLength - trueDayLength) < 1.5);
        }
    }
}


void TestCalendar::testFirstDateNoneNoFollower() {
    createSimulation("test_calendar_no_follower.xml", 0);

    try {
        sim->execute();
        QString msg = "Should not execute";
        QFAIL(qPrintable(msg));
    }
    catch (Exception &ex) {
    }
}

void TestCalendar::testFirstDateNoneWithFollowerWithoutFirstDate() {
    createSimulation("test_calendar_with_follower.xml", 1);

    try {
        sim->execute();
        QString msg = "Should not execute";
        QFAIL(qPrintable(msg));
    }
    catch (Exception &ex) {
    }
}

void TestCalendar::testFirstDateNoneWithFollowerWithFirstDate() {
    createSimulation("test_calendar_with_follower.xml", 1);

    weather->changeParameter("firstDate", WEATHER_DATE);

    try {
        sim->execute();
        QCOMPARE(calendar->parameter<QDate>("firstDate"), WEATHER_DATE);
        QCOMPARE(finalCalendarDate(), local::finalDate(WEATHER_DATE));
        QCOMPARE(local::finalWeatherDate(), local::finalDate(WEATHER_DATE));
    }
    catch (Exception &ex) {
        delete sim;
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestCalendar::testFirstDatePresentNoFollower() {
    createSimulation("test_calendar_no_follower.xml", 1);

    calendar->changeParameter("firstDate", QDate(2010, 4, 1));

    try {
        sim->execute();
        QCOMPARE(finalCalendarDate(), local::finalDate(QDate(2010, 4, 1)));
    }
    catch (Exception &ex) {
        delete sim;
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestCalendar::testFirstDatePresentFollowerWithout() {
    createSimulation("test_calendar_with_follower.xml", 1);

    calendar->changeParameter("firstDate", WEATHER_DATE);

    QVERIFY(weather->parameter<QDate>("firstDate").isNull());

    try {
        sim->execute();
        QCOMPARE(weather->parameter<QDate>("firstDate"), WEATHER_DATE);
        QCOMPARE(finalCalendarDate(), local::finalDate(WEATHER_DATE));
        QCOMPARE(local::finalWeatherDate(), local::finalDate(WEATHER_DATE));
    }
    catch (Exception &ex) {
        delete sim;
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestCalendar::testFirstDatePresentFollowerBefore() {
    createSimulation("test_calendar_with_follower.xml", 1);

    calendar->changeParameter("firstDate", QDate(2010, 4, 1));
    weather->changeParameter("firstDate", WEATHER_DATE);

    try {
        sim->execute();
        QCOMPARE(finalCalendarDate(), local::finalDate(QDate(2010, 4, 1)));
        QCOMPARE(local::finalWeatherDate(), local::finalDate(QDate(2010, 4, 1)));
    }
    catch (Exception &ex) {
        delete sim;
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestCalendar::testFirstDatePresentFollowerSame() {
    createSimulation("test_calendar_with_follower.xml", 1);

    calendar->changeParameter("firstDate", WEATHER_DATE);
    weather->changeParameter("firstDate", WEATHER_DATE);

    try {
        sim->execute();
        QCOMPARE(finalCalendarDate(), local::finalDate(WEATHER_DATE));
        QCOMPARE(local::finalWeatherDate(), local::finalDate(WEATHER_DATE));
    }
    catch (Exception &ex) {
        delete sim;
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestCalendar::testFirstDatePresentFollowerAfter() {
    createSimulation("test_calendar_with_follower.xml", 1);

    calendar->changeParameter("firstDate", WEATHER_DATE.addDays(-10));
    weather->changeParameter("firstDate", WEATHER_DATE);

    try {
        sim->execute();
        QCOMPARE(calendar->parameter<QDate>("firstDate"), WEATHER_DATE);
        QCOMPARE(finalCalendarDate(), local::finalDate(WEATHER_DATE));
        QCOMPARE(local::finalWeatherDate(), local::finalDate(WEATHER_DATE));
    }
    catch (Exception &ex) {
        delete sim;
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}

void TestCalendar::testFirstDateFollowersConflicting() {
    createSimulation("test_calendar_with_two_followers.xml", 2);

    Model *weather2 = seekOneDescendant<Model*>("weather2", 0);

    calendar->changeParameter("firstDate", QDate(2010, 4, 1));
    weather->changeParameter("firstDate", QDate(2010, 6, 15));
    weather2->changeParameter("firstDate", QDate(2010, 6, 16));

    try {
        sim->execute();
        QString msg = "Should not execute";
        QFAIL(qPrintable(msg));
    }
    catch (Exception &ex) {
    }
}

void TestCalendar::testFirstDateFollowersNotConflicting() {
    createSimulation("test_calendar_with_two_followers.xml", 2);

    Model *weather2 = seekOneDescendant<Model*>("weather2", 0);

    calendar->changeParameter("firstDate", QDate(2010, 4, 1));
    weather->changeParameter("firstDate", WEATHER_DATE);
    weather2->changeParameter("firstDate", WEATHER_DATE);

    try {
        sim->execute();
    }
    catch (Exception &ex) {
        delete sim;
        QString msg = "Could not execute: " + ex.message();
        QFAIL(qPrintable(msg));
    }
}


