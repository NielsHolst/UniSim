#include <test_models/with_parameter_file.h>
#include <usbase/exception.h>
#include <usbase/parameter_file.h>
#include <usengine/simulation.h>
#include <usengine/simulation_maker.h>
#include "test_parameter_file.h"

using std::cout;
using namespace UniSim;

void TestParameterFile::test1D() {
    typedef ParameterFile1D<QString> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_1d_input.txt"), 0, QString());
    QCOMPARE(p->value<double>("barley", "nitrogen"), 90.1);
    QCOMPARE(p->value<int>("maize", "water"), 85);
    delete p;
}

void TestParameterFile::test2D() {
    typedef ParameterFile2D<QString, QString> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_2d_input.txt"), 0, QString());
    QCOMPARE(p->value<double>("barley","sandy","nitrogen"), 90.1);
    QCOMPARE(p->value<int>("barley","sandy","water"), 70);
    QCOMPARE(p->value<double>("barley","clay","nitrogen"), 95.4);
    QCOMPARE(p->value<int>("barley","clay","water"), 85);
    QCOMPARE(p->value<bool>("wheat","clay","isCase"), true);
    QCOMPARE(p->value<bool>("wheat","sandy","isCase"), false);
    delete p;
}

void TestParameterFile::test3D() {
    typedef ParameterFile3D<QString, QString, int> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_3d_input.txt"), 0, QString());
    QCOMPARE(p->value<bool>("wheat","clay",1,"isCase"), true);
    delete p;
}

void TestParameterFile::test4D() {
    typedef ParameterFile4D<QString, QString, int, bool> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_4d_input.txt"), 0, QString());
    QCOMPARE(p->value<int>("wheat","clay",1,true,"water"), 76);
    delete p;
}

void TestParameterFile::testOneLineFile() {
    typedef ParameterFile2D<QString, QString> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_one_line_input.txt"), 0, QString());
    QCOMPARE(p->value<int>("barley","sandy","treatment"), 1);
    delete p;
}

void TestParameterFile::testMissingFile() {
    bool excepted = false;
    typedef ParameterFile1D<QString> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_missing_file_input.txt"), 0, QString());
    try {
        p->value<double>("barley", "nitrogen");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
    delete p;
}

void TestParameterFile::testEmptyFile() {
    bool excepted = false;
    typedef ParameterFile1D<QString> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_empty_file_input.txt"), 0, QString());
    try {
        p->value<double>("barley", "nitrogen");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
    delete p;
}

void TestParameterFile::testIllFormedFile() {
    bool excepted = false;
    typedef ParameterFile1D<QString> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_ill_formed_file_input.txt"), 0, QString());
    try {
        p->value<double>("barley", "nitrogen");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
    delete p;
}

void TestParameterFile::testOnlyColumnHeadings() {
    bool excepted = false;
    typedef ParameterFile1D<QString> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_only_column_headings_input.txt"), 0, QString());
    try {
        p->value<double>("barley", "nitrogen");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
    delete p;
}

void TestParameterFile::testWrongKeyType() {
    bool excepted = false;
    typedef ParameterFile1D<bool> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_only_column_headings_input.txt"), 0, QString());
    try {
        p->value<double>("barley", "nitrogen");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
    delete p;
}

void TestParameterFile::testWrongColumnType() {
    bool excepted = false;
    typedef ParameterFile1D<bool> Par;
    Par *p = new Par("Test", filePath("test_parameter_file_wrong_column_type_input.txt"), 0, QString());
    try {
        p->value<double>("barley", "nitrogen");
    }
    catch (Exception &) {
        excepted = true;
    }
    QVERIFY(excepted);
    delete p;
}

void TestParameterFile::testInXmlModel() {
    Simulation *simulation = 0;
    SimulationMaker maker;
    try {
        simulation = maker.parse(filePath("model_with_parameter_file.xml"));
        setSimulationObject(simulation);
    }
    catch (const Exception &ex) {
        QString msg = "Unexpected exception: " + ex.message();
        QFAIL(qPrintable(msg));
    }

    typedef test::WithParameterFile::CropParameters Crops;
    test::WithParameterFile *model = simulation->seekOneDescendant<test::WithParameterFile*>("withParameterFile");
    Crops *crops = model->theCrops();
    QCOMPARE(crops->value<double>("barley","sandy","nitrogen"), 90.1);
    QCOMPARE(crops->value<int>("barley","sandy","water"), 70);

    delete simulation;
}

QString TestParameterFile::filePath(QString fileName) {
    return QFileInfo(inputFolder(), fileName).absoluteFilePath();
}

QDir TestParameterFile::inputFolder() {
    QDir testDir = FileLocations::location(FileLocationInfo::Models);
    testDir.cdUp();
    testDir.cdUp();
    testDir.cd("src/library/usbase/test/input");
    Q_ASSERT(testDir.exists());
    return testDir;
}




