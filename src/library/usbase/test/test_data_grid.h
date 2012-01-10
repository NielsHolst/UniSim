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
    void test1D();
    void test2D();
    void testOneLineFile();
    void testMissingFile();
    void testEmptyFile();
    void testIllFormedFile();
    void testOnlyColumnHeadings();
    void testWrongKeys();
    void testRowIndices();
private:
    QString filePath(QString fileName);
    QDir inputFolder();
};

DECLARE_TEST(TestDataGrid)

#endif 
