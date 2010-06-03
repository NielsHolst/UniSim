#include <QObject>
#include <usbase/model.h>
#include "../model_maker.h"
#include "test_model_maker.h"

using namespace UniSim;

void TestModelMaker::testCreation()
{
	QVERIFY(ModelMaker::canCreate("LifeCycle"));
	QVERIFY(ModelMaker::canCreate("LifeStage"));
	QVERIFY(!ModelMaker::canCreate("No way"));

	Model *model = 	ModelMaker::create("LifeCycle", "weed", 0);
	QVERIFY(model);
    QCOMPARE(model->metaObject()->className(), "test::LifeCycle");
	QCOMPARE(model->metaObject()->superClass()->className(), "UniSim::Model");
	QCOMPARE(model->objectName(), QString("weed"));
	QVERIFY(model->parent()==0);

	Model *seed = dynamic_cast<Model*>( ModelMaker::create("LifeStage", "seed", model) );
	ModelMaker::create("LifeStage", "juvenile", model);
	ModelMaker::create("LifeStage", "mature", model);

	QVERIFY(seed);
	QVERIFY(seed->parent()==model);
    QCOMPARE(model->seekChildren<Model*>("*").size(), 3);

    seed->setRecursionPolicy(Model::Update, Model::ChildrenNot);
	QMap<QString, QString> param;
	param["test"] = QString::number(30);
				
	delete model;
}
 
