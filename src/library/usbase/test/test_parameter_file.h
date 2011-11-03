#ifndef UNISIM_TEST_PARAMETERS_FILE_H
#define UNISIM_TEST_PARAMETERS_FILE_H

#include <QDir>
#include <QObject>
#include <usbase/test/autotest.h>
	
namespace UniSim {
    class Model;
}

class TestParameterFile : public QObject
{
    Q_OBJECT
    
private slots:
    void test1D();
    void test2D();
    void test3D();
    void test4D();
    void testOneLineFile();
    void testMissingFile();
    void testEmptyFile();
    void testIllFormedFile();
    void testOnlyColumnHeadings();
    void testWrongKeyType();
    void testWrongColumnType();
    void testInXmlModel();
private:
    QString filePath(QString fileName);
    QDir inputFolder();
};

DECLARE_TEST(TestParameterFile)

#endif 
