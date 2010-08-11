#include <limits>
#include <cmath>
#include <iostream>
#include <QFile>
#include <QObject>
#include <usbase/model.h>
#include <usbase/utilities.h>
#include <usengine/model_maker.h>
#include "test_life_cycle.h"

using namespace std;
using namespace UniSim;

void TestLifeCycle::initTestCase()
{
}

void TestLifeCycle::cleanupTestCase()
{
}

void TestLifeCycle::testUpdate()
{
	Model *weed = ModelMaker::create("LifeCycle", "biannual");
	QVERIFY(weed);
	
	Model *seedling = ModelMaker::create("LifeStage", "seedling", weed);
	QVERIFY(seedling);
	Model *juvenile = ModelMaker::create("LifeStage", "juvenile", weed);
	QVERIFY(juvenile);
	Model *mature = ModelMaker::create("LifeStage", "mature", weed);
	QVERIFY(mature);
    QCOMPARE(weed->seekChildren<Model*>("*").size(), 3);

    weed->deepInitialize();
	
	seedling->changeParameter("k", 30);	
	seedling->changeParameter("duration", 50.);	
	
	juvenile->changeParameter("k", 20);	
	juvenile->changeParameter("duration", 200.);	
	
	mature->changeParameter("k", 10);	
	mature->changeParameter("duration", 1550.);	
	
    weed->deepReset();
	
    Model *first = seekOneChild<Model*>("seedling", weed);
    Model *last = seekOneChild<Model*>("mature", weed);
    QVERIFY(first);
    QVERIFY(last);
	
	double myInput = 1000;
    first->pushVariable("input", myInput);
	
    static double EPS = std::min(myInput*1000*std::numeric_limits<double>::epsilon(),  1e-6);

	for (int i = 0; i < 10000; ++i) {
		weed->update();
	}

    QVERIFY2((fabs(first->pullVariable<double>("inputTotal") - myInput) < EPS),
			qPrintable("Expected: "+QString::number(myInput) 
            + " Got: "+QString::number(first->pullVariable<double>("inputTotal"))
            + " Diff: "+QString::number(fabs(first->pullVariable<double>("inputTotal") - myInput))
			+ " > "+QString::number(EPS) 
			));
    QVERIFY2((fabs(first->pullVariable<double>("outputTotal") - myInput) < EPS),
			qPrintable("Expected: "+QString::number(myInput) 
            + " Got: "+QString::number(first->pullVariable<double>("outputTotal"))
            + " Diff: "+QString::number(fabs(first->pullVariable<double>("outputTotal") - myInput))
			+ " > "+QString::number(EPS) 
			));
    QVERIFY2((fabs(last->pullVariable<double>("outputTotal") - myInput) < EPS),
			qPrintable("Expected: "+QString::number(myInput) 
            + " Got: "+QString::number(last->pullVariable<double>("outputTotal"))
            + " Diff: "+QString::number(fabs(last->pullVariable<double>("outputTotal") - myInput))
			+ " > "+QString::number(EPS) 
			));

    weed->deepReset();
    first->pushVariable("input", myInput);
	for (int i = 0; i < 10000; ++i) {
        weed->deepUpdate();
	}

    QVERIFY2((fabs(first->pullVariable<double>("inputTotal") - myInput) < EPS),
			qPrintable("Expected: "+QString::number(myInput) 
            + " Got: "+QString::number(first->pullVariable<double>("inputTotal"))
            + " Diff: "+QString::number(fabs(first->pullVariable<double>("inputTotal") - myInput))
			+ " > "+QString::number(EPS) 
			));
    QVERIFY2((fabs(first->pullVariable<double>("outputTotal") - myInput) < EPS),
			qPrintable("Expected: "+QString::number(myInput) 
            + " Got: "+QString::number(first->pullVariable<double>("outputTotal"))
            + " Diff: "+QString::number(fabs(first->pullVariable<double>("outputTotal") - myInput))
			+ " > "+QString::number(EPS) 
			));
    QVERIFY2((fabs(last->pullVariable<double>("outputTotal") - myInput) < EPS),
			qPrintable("Expected: "+QString::number(myInput) 
            + " Got: "+QString::number(last->pullVariable<double>("outputTotal"))
            + " Diff: "+QString::number(fabs(last->pullVariable<double>("outputTotal") - myInput))
			+ " > "+QString::number(EPS) 
			));


	delete weed;
}
 
