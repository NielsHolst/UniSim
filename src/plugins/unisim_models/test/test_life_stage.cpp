#include <cmath>
#include <iostream>
#include <limits>
#include <QFile>
#include <QObject>
#include <usbase/exception.h>
#include <usbase/model.h>
#include <usengine/model_maker.h>
// #include <usutils/frequency_distribution.h>  put it where?
#include "test_life_stage.h"

//using namespace NHolst;

void TestLifeStage::initTestCase()
{
}

void TestLifeStage::cleanupTestCase()
{
}

void TestLifeStage::testUpdate()
{
	const double L = 50, input = 200;
	const int k = 30;
	
	UniSim::Model *stage = UniSim::ModelMaker::create("LifeStage", "flowering");
	QVERIFY(stage);
	stage->initialize();
	try { 
		stage->changeParameter("k", k);
		stage->changeParameter("duration", L);
	}
	catch (const UniSim::Exception &ex) {
		QWARN(qPrintable(ex.message()));
		QVERIFY(false);
	}
	
	stage->reset();
    stage->pushVariable("input", input);

    QWARN("Use FrequencyDistribution here !");
    /*

    FrequencyDistribution dist;
	
	for (int i = 0; i < 1000; ++i) {
		stage->update();
		dist.add(i, stage->state("output"));
	}	
	
	static double EPS = 1000.*std::numeric_limits<double>::epsilon();
	QVERIFY2(fabs(stage->state("inputTotal") - input) < EPS, 
			qPrintable("Expected: "+QString::number(input)+" Got: "+QString::number(stage->state("inputTotal"))));
			
	QVERIFY2(fabs(dist.average() - L) < EPS, 
			qPrintable("Expected: "+QString::number(L)+" Got: "+QString::number(dist.average())));

	QVERIFY2(fabs(stage->state("outputTotal") - input) < EPS, 
			qPrintable("Expected: "+QString::number(input)+" Got: "+QString::number(stage->state("outputTotal"))));
			
    std::cout << "Known issue: Variance of distributed delay does not match: " << dist.variance() << " " << L*L/k << "\n";
*/

	delete stage;
	
}
 
void TestLifeStage::testState()
{
	const double L = 50, input = 200;
	const int k = 30;
	
	UniSim::Model *stage = UniSim::ModelMaker::create("LifeStage", "senescent");
	QVERIFY(stage);
	stage->initialize();
	stage->changeParameter("k", k);
	stage->changeParameter("duration", L);
	
	stage->reset();
    stage->pushVariable("input", input);
	
	static double EPS = std::min(input*1000.*std::numeric_limits<double>::epsilon(), 1e-6);
	
	for (int i = 0; i < 1000; ++i) {
		stage->update();
        QVERIFY2(fabs( stage->pullVariable<double>("contents") +
                       stage->pullVariable<double>("outputTotal") -
                       stage->pullVariable<double>("inputTotal") ) < EPS,

                qPrintable("stage->inputTotal: "
                    + QString::number(stage->pullVariable<double>("inputTotal"))
                    + " Got (contents + outputTotal): "
                    + QString::number(stage->pullVariable<double>("contents")) + " + "
                    + QString::number(stage->pullVariable<double>("outputTotal")) + " = "
                    + QString::number(stage->pullVariable<double>("contents") +
                                      stage->pullVariable<double>("outputTotal"))
                    + " Diff: " + QString::number(stage->pullVariable<double>("contents") +
                                                  stage->pullVariable<double>("outputTotal") -
                                                  stage->pullVariable<double>("inputTotal"))
				));
	}
	

	delete stage;
}
