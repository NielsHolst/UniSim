#include <iostream>
#include <QDate>
#include <QString>
#include <QVariant>
#include "../exception.h"
#include "../identifier.h"
#include "../parameters.h"
#include "test_parameters.h"

using UniSim::Exception;

void TestParameters::testSetIntWithStringValue() {
    try {
        UniSim::Parameters p;
        QString name("test_int");
        p.initParameter(name,  QString("123"));

        int i;
        p.setParameter(name, &i, 456);
        QCOMPARE(i,123);
        UniSim::Identifier id = UniSim::Identifier(name);
        QCOMPARE(p.parameter<int>(id),123);
    }
    catch (const Exception &ex) {
        QWARN(qPrintable(ex.message()));
        QVERIFY(false);
    }

}

void TestParameters::testSetBoolWithStringValue() {
	UniSim::Parameters p;
	QString name("test");
    p.initParameter(name,  QString("Yes"));
	
	bool b;
	p.setParameter(name, &b, false);
	QCOMPARE(b,true);
	QCOMPARE(p.parameter<bool>(name),true);	
}

void TestParameters::testSetStringWithStringValue() {
	UniSim::Parameters p;
	QString name("test_string");
    p.initParameter(name,  QString("top"));
	
	QString s;
	p.setParameter(name, &s, QString("toppest"));
	QCOMPARE(s,QString("top"));
	QCOMPARE(p.parameter<QString>(name), QString("top"));	
}

void TestParameters::testSetDateWithStringValue() {
	UniSim::Parameters p;
	QString name("test_date");
	QDate testDate = QDate(2008, 12, 23);
    p.initParameter(name,  "23/12/2008");
	
	QDate date;
	p.setParameter(name, &date, QDate(2009, 1, 20));
	QCOMPARE(date, testDate);
	QCOMPARE(p.parameter<QDate>(name), testDate);		
}


void TestParameters::testSetIntWithoutStringValue() {
	UniSim::Parameters p;
	QString name("test_int");
	
	int i;
	p.setParameter(name, &i, 456);
	QCOMPARE(i,456);
	QCOMPARE(p.parameter<int>(name),456);	
}	

void TestParameters::testSetBoolWithoutStringValue() {
	UniSim::Parameters p;
	QString name("test");
	
	bool b = false;
	p.setParameter(name, &b, true);
	QCOMPARE(b,true);
	QCOMPARE(p.parameter<bool>(name),true);	
}	

void TestParameters::testSetStringWithoutStringValue() {
	UniSim::Parameters p;
	QString name("test_string");
	
	QString s;
	p.setParameter(name, &s, QString("toppest"));
	QCOMPARE(s,QString("toppest"));
	QCOMPARE(p.parameter<QString>(name), QString("toppest"));	
}
	
void TestParameters::testSetDateWithoutStringValue() {
	UniSim::Parameters p;
	QString name("test_date");
	QDate testDate = QDate(2008, 12, 23);
	
	QDate date;
	p.setParameter(name, &date, testDate);
	QCOMPARE(date, testDate);
	QCOMPARE(p.parameter<QDate>(name), testDate);		
}

void TestParameters::testChangeInt(){
	UniSim::Parameters p;
	QString name("test");
	
	int i;
	p.setParameter(name, &i, 123);
	p.changeParameter(name, 456);
	QCOMPARE(i,456);
	QCOMPARE(p.parameter<int>(name), 456);	
}

void TestParameters::testChangeBool(){
	UniSim::Parameters p;
	QString name("test");
	
	bool b;
	p.setParameter(name, &b, true);
	p.changeParameter(name, false);
	QCOMPARE(b,false);
	QCOMPARE(p.parameter<bool>(name), false);	
}

void TestParameters::testChangeString() {
	UniSim::Parameters p;
	QString name("test");
	
	QString s;
	p.setParameter(name, &s, QString("top"));
	p.changeParameter(name, QString("toppest"));
	QCOMPARE(s, QString("toppest"));
	QCOMPARE(p.parameter<QString>(name), QString("toppest"));	
}

void TestParameters::testChangeDate() {
	UniSim::Parameters p;
	QString name("test");
	
	QDate date;
	p.setParameter(name, &date, QDate(2008, 12, 23));

	QDate newDate = QDate(2008, 10, 21);
	p.changeParameter(name, newDate);
	QCOMPARE(date, newDate);
	QCOMPARE(p.parameter<QDate>(name), newDate);		
}


void TestParameters::testSetSameTwice() {
	UniSim::Parameters p;
	QString name("test_int");
    p.initParameter(name,  QString("123"));
	
	bool excepted = false;
	try {
        p.initParameter(name,  QString("123"));
	}
	catch (const Exception &ex) {
		excepted = true;
		bool correctMessage = ex.message().contains(name) && ex.message().contains("twice");
		QVERIFY2(correctMessage, qPrintable("Incorrect error message: "+ex.message()));
	}
	QVERIFY(excepted);
}

void TestParameters::testConflictingStringValueType() {
	UniSim::Parameters p;
	QString name("test_int");
    p.initParameter(name,  QString("abc"));

	bool excepted = false;
	try {
		int i;
		p.setParameter(name, &i, 456);
	}
	catch (const Exception &ex) {
		excepted = true;
		bool correctMessage = ex.message().contains("convert");
		QVERIFY2(correctMessage, qPrintable("Incorrect error message: "+ex.message()));
	}
	QVERIFY(excepted);
}

void TestParameters::testMissing() {
    UniSim::Parameters p;
    try {
        p.parameter<int>("missing");
        QFAIL("Missing parameter not detected. Exception expected.");
    }
    catch (const Exception &ex) {
    }
}

void TestParameters::testNotSet() {
    UniSim::Parameters p;
    QString name("test_int");
    p.initParameter(name,  QString("100"));

    try {
        QString value = p.parameter<QString>(name);
        QCOMPARE(value, QString("100"));
    }
    catch (const Exception &ex) {
        QFAIL("Could not retrieve QString value from unset parameter");
    }

    try {
        int value = p.parameter<int>(name);
        QCOMPARE(value, 100);
    }
    catch (const Exception &ex) {
        QFAIL("Could not retrieve int value from unset parameter");
    }
}

