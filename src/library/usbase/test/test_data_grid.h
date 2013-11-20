#ifndef UNISIM_TEST_DATA_GRID_H
#define UNISIM_TEST_DATA_GRID_H

#include <QDir>
#include <QObject>
#include <usbase/test/autotest.h>
	
namespace UniSim {
    class Model;
}

class TestDataGrid : public QObject
{
    Q_OBJECT
    
private slots:
    void testNoKeys();
    void test1D();
    void test2D();
    void testOneLineFile();
    void testMissingFile();
    void testEmptyFile();
    void testIllFormedFile();
    void testOnlyColumnHeadings();
    void testAutoColumnHeadingMissing();
    void testAutoColumnHeadingEmpty();
    void testWrongKeys();
    void testRowIndices();
    void testColumnString();
    void testColumnInt();
private:
    QString filePath(QString fileName);
    QDir inputFolder();
};

DECLARE_TEST(TestDataGrid)

#endif 
