#include <iostream>
#include <QDir>
#include <usbase/clock.h>
#include <usbase/file_locations.h>
#include <usbase/model.h>
#include <usengine/model_maker.h>
#include <usengine/simulation_maker.h>
#include "test_calendar.h"

using std::cout;
using namespace UniSim;

const QDate WEATHER_DATE = QDate(2009, 9, 1);

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
    dates.append(QDate(2010, 1,15));
    dates.append(QDate(2010, 4,15));
    dates.append(QDate(2010, 7,15));
    dates.append(QDate(2010,10,15));

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

void TestCalendar::init() {
    calendar = ModelMaker::create("UniSim::Calendar", "calendar", 0);
    calendar->deepInitialize();
}

void TestCalendar::cleanup() {
    delete calendar;
    calendar = 0;
}

void TestCalendar::testSolarElevation() {
    for (int lo = 0; lo < locations.size(); ++lo) {
        calendar->seekOneChild<Parameter<double>*>("latitude") -> setValue(locations[lo].latitude);
        calendar->seekOneChild<Parameter<QDate>*>("initialDate") -> setValue(QDate(2010, 1, 1));
        calendar->deepReset();
        for (int da = 0; da < dates.size(); ++da) {
            while (calendar->pullVariable<QDate>("date") < dates[da]) {
                calendar->update();
            }
            for (int ho = 0; ho < hours.size(); ++ho) {
                double astroHour = hours[ho] - solarNoonDiff[lo][da];
                clock()->doTick(astroHour);
                double sinb = calendar->pullVariable<double>("sinb");
                double estSolarElev = asin(sinb)/PI*180.;
                QVERIFY(fabs(estSolarElev - solarElev[lo][da][ho]) < 1.);
            }
        }
    }
}


void TestCalendar::testDayLength() {
    for (int lo = 0; lo < locations.size(); ++lo) {
        calendar->seekOneChild<Parameter<double>*>("latitude") -> setValue(locations[lo].latitude);
        calendar->seekOneChild<Parameter<QDate>*>("initialDate") -> setValue(QDate(2010, 1, 1));
        calendar->deepReset();
        for (int da = 0; da < dates.size(); ++da) {
            while (calendar->pullVariable<QDate>("date") < dates[da]) {
                calendar->update();
            }
        double trueDayLength = QTime(0,0,0).secsTo(dayLength[lo][da])/60./60.;
        double estDayLength = calendar->pullVariable<double>("dayLength");
        cout << "Day length deviation (h)" << lo << ": " << (estDayLength - trueDayLength) << "\n";
        QVERIFY(fabs(estDayLength - trueDayLength) < 0.5);
        }
    }
}


void TestCalendar::testDaySteps() {
    calendar->deepReset();
    QDate initialDate = calendar->parameter<QDate>("initialDate");
    QDate date = calendar->pullVariable<QDate>("date");
    QCOMPARE(initialDate.addDays(-1), date);

    const int n = 7;
    for (int i = 0; i < n; ++i){
        calendar->deepUpdate();
    }
    QCOMPARE(initialDate.addDays(n-1), calendar->pullVariable<QDate>("date"));
}

void TestCalendar::testHourSteps() {
    calendar->seekOneChild<Parameter<QTime>*>("initialTimeOfDay") -> setValue(QTime(12,0));
    calendar->seekOneChild<Parameter<char>*>("timeUnit") -> setValue('h');
    calendar->seekOneChild<Parameter<int>*>("timeStep") -> setValue(4);
    calendar->deepReset();

    QDate initialDate = calendar->parameter<QDate>("initialDate");
    QDate date = calendar->pullVariable<QDate>("date");
    QTime time = calendar->pullVariable<QTime>("timeOfDay");
    QCOMPARE(initialDate, date);
    QCOMPARE(time, QTime(8,0));

    const int n = 8;
    for (int i = 0; i < n; ++i){
        calendar->deepUpdate();
    }

    QCOMPARE(calendar->pullVariable<QDate>("date"), initialDate.addDays(1));
    QCOMPARE(calendar->pullVariable<QTime>("timeOfDay"), QTime(16,0));
}

