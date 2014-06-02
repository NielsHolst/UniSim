#include <iostream>
#include <QDir>
#include <usbase/exception.h>
#include <usengine/ast.h>
#include "test_ast.h"

using namespace UniSim;
using namespace grammar;
using std::cout;

namespace {
    QString filePath(QString fileName) {
        QDir uniSimDir = QDir("../..");
        return uniSimDir.absolutePath() +
                "/src/library/usengine/test/input/" +
                fileName;
    }
}

void TestAst::testOptionalParts() {
	QList<QString> expressions;
	expressions 
		<< "Simulation()"
		<< "Simulation()[]"
		<< "Simulation()[[]]"
		<< "Simulation()[][[]]"
		<< "Simulation()[[]][]"

		<< "Simulation(){}"
		<< "Simulation()[]{}"
		<< "Simulation()[[]]{}"
		<< "Simulation()[][[]]{}"
        << "Simulation()[[]][]{}"
    ;
	for (auto s : expressions) {
        Ast ast;
        Model *model;
        try {
            model = ast.createModel(s);
            QVERIFY(model);
            QCOMPARE(model->objectDeclaration.className, std::string("Simulation"));
            QVERIFY(model->objectDeclaration.objectName.empty());
            QVERIFY(model->input.empty());
            QVERIFY(model->output.init.empty());
            QVERIFY(model->output.reset.empty());
            QVERIFY(model->modelList.empty());
            delete model;
        }
        catch (Exception &ex) {
            QFAIL(qPrintable(ex.message()));
        }
    }
}
	
void TestAst::testMissingParts() {
    QList<QString> expressions;
    expressions
        << ""
        << "()"
        << "Simulation{}"
        << "Simulation[]"
        << "Simulation[[]]"
    ;
    for (auto s : expressions) {
        Ast ast;
        Model *model;
        try {
            model = ast.createModel(s);
            QVERIFY(!model);
        }
        catch (Exception &ex) {
            QFAIL(qPrintable(ex.message()));
        }
    }
}

void TestAst::testKeyValue() {
    Ast ast;
    Model *model;
    try {
        model = ast.createModel("Sim(p=\"a b\")"); QVERIFY(model);
        KeyValue k1{"p", "a b", "="};
        QCOMPARE(model->input.at(0), k1);
        delete model;

        model = ast.createModel("Sim(\"a b\")"); QVERIFY(model);
        KeyValue k2{"", "a b", "="};
        QCOMPARE(model->input.at(0), k2);
        delete model;

        model = ast.createModel("Sim(p->x[y])"); QVERIFY(model);
        KeyValue k3{"p", "x[y]", "->"};
        QCOMPARE(model->input.at(0), k3);
        delete model;

        model = ast.createModel("Sim(->x[y])"); QVERIFY(model);
        KeyValue k4{"", "x[y]", "->"};
        QCOMPARE(model->input.at(0), k4);
        delete model;

        model = ast.createModel("Sim(p=\"a b\", \"a b\", p->x[y], ->x[y])"); QVERIFY(model);
        KeyValueList list{k1, k2, k3, k4};
        QCOMPARE(model->input, list);
        delete model;

        model = ast.createModel("Sim()[p=\"a b\", \"a b\", p->x[y], ->x[y]])"); QVERIFY(model);
        QCOMPARE(model->output.reset, list);
        delete model;

        model = ast.createModel("Sim()[[p=\"a b\", \"a b\", p->x[y], ->x[y]]])"); QVERIFY(model);
        QCOMPARE(model->output.init, list);
        delete model;
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }
}

void TestAst::testNestedModels() {
    QString s =
            "Simulation animal() {"
                "Mammal whale(a=b)"
                "Insect wasp(c=d) {"
                    "Stage egg()"
                "}"
            "}";
    Ast ast;
    Model *model;
    try {
        model = ast.createModel(s); QVERIFY(model);
        QCOMPARE(model->child(0)->objectDeclaration.className, std::string("Mammal"));
        QCOMPARE(model->child(0)->objectDeclaration.objectName, std::string("whale"));
        QCOMPARE(model->child(1)->objectDeclaration.className, std::string("Insect"));
        QCOMPARE(model->child(1)->objectDeclaration.objectName, std::string("wasp"));
        QCOMPARE(model->child(1)->child(0)->objectDeclaration.objectName, std::string("egg"));
        delete model;
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.message()));
    }

}

void TestAst::testUniFile() {

}

void TestAst::testInputNotInOrder() {

}

void TestAst::testOutputResetNotInOrder() {

}

void TestAst::testOutputInitNotInOrder() {

}
