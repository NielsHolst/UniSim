#ifndef UNISIM_TEST_DATASET_H
#define UNISIM_TEST_DATASET_H

#include <QFile>
#include <QStringList>
#include <QVector>

namespace UniSim{

class TestDataset
{
public:
    TestDataset(int rows, int columns);
    void read(QString fileName);
    QStringList labels() const;
    double value(int row, int col) const;

private:
    void openFile(QString fileName);
    void readLineItems();
    void readData();

    int rows, columns;
    QString filePath;
    QFile file;
    QStringList _labels, lineItems;
    QVector<double> data;
};

}//namespace

#endif
