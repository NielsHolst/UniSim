#include <usbase/time.h>
#include "test_time.h"

using namespace UniSim;
	
void TestTime::testConversion() {
    QCOMPARE(Time::unitToChar(Time::charToUnit('s')), 's');
    QCOMPARE(Time::unitToChar(Time::charToUnit('m')), 'm');
    QCOMPARE(Time::unitToChar(Time::charToUnit('h')), 'h');
    QCOMPARE(Time::unitToChar(Time::charToUnit('d')), 'd');
    QCOMPARE(Time::unitToChar(Time::charToUnit('y')), 'y');
}

void TestTime::testAddSeconds() {
    QDateTime date1(QDate(2010, 3, 1), QTime(), Qt::UTC), date2;
    int oneDay = 24*60*60;

    date2 = date1 + Time(oneDay-1, Time::Seconds);
    QCOMPARE(date1.date(), date2.date());

    date2 = date1 + Time(oneDay, Time::Seconds);
    QCOMPARE(date1.addDays(1).date(), date2.date());

    date2 = date1 + Time(oneDay+7, Time::Seconds);
    QCOMPARE(date1.addDays(1).date(), date2.date());
    QCOMPARE(date2.time().second(), 7);
}

void TestTime::testAddMinutes() {
    QDateTime date1(QDate(2010, 3, 1), QTime(), Qt::UTC), date2;
    int oneDay = 24*60;

    date2 = date1 + Time(oneDay-1, Time::Minutes);
    QCOMPARE(date1.date(), date2.date());

    date2 = date1 + Time(oneDay, Time::Minutes);
    QCOMPARE(date1.addDays(1).date(), date2.date());

    date2 = date1 + Time(oneDay+7, Time::Minutes);
    QCOMPARE(date1.addDays(1).date(), date2.date());
    QCOMPARE(date2.time().minute(), 7);
}

void TestTime::testAddHours() {
    QDateTime date1(QDate(2010, 3, 1), QTime(), Qt::UTC), date2;
    int oneDay = 24;

    date2 = date1 + Time(oneDay-1, Time::Hours);
    QCOMPARE(date1.date(), date2.date());

    date2 = date1 + Time(oneDay, Time::Hours);
    QCOMPARE(date1.addDays(1).date(), date2.date());

    date2 = date1 + Time(oneDay+7, Time::Hours);
    QCOMPARE(date1.addDays(1).date(), date2.date());
    QCOMPARE(date2.time().hour(), 7);
}

void TestTime::testAddDays() {
    QDateTime date1(QDate(2010, 3, 1), QTime(), Qt::UTC), date2;
    int oneDay = 1;

    date2 = date1 + Time(oneDay-1, Time::Days);
    QCOMPARE(date1.date(), date2.date());

    date2 = date1 + Time(oneDay, Time::Days);
    QCOMPARE(date1.addDays(1).date(), date2.date());

    date2 = date1 + Time(oneDay+7, Time::Days);
    QCOMPARE(date1.addDays(oneDay+7).date(), date2.date());
    QCOMPARE(date2.date().day(), 9);

}

void TestTime::testAddYears() {
    QDateTime date1(QDate(2010, 3, 1), QTime(), Qt::UTC), date2;

    date2 = date1 + Time(0, Time::Years);
    QCOMPARE(date1, date2);

    date2 = date1 + Time(1, Time::Years);
    QCOMPARE(date1.addDays(365), date2);

    date2 = date1 + Time(7, Time::Years);
    int leapDays = 2;
    QCOMPARE(date1.addDays(7*365 + leapDays), date2);
}



