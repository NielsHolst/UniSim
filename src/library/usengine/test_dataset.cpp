#include <usbase/exception.h>
#include <usbase/file_locations.h>
#include "test_dataset.h"

namespace UniSim {


TestDataset::TestDataset(int rows_, int columns_)
    : rows(rows_), columns(columns_)
{
}

void TestDataset::read(QString fileName) {
    const int N = rows*columns;
    openFile(fileName);
    data.clear();
    data.reserve(N);
    readData();
    if (data.size() != N)
        throw Exception("Wrong dataset size. Expected: " + QString::number(N) +
                        ". Got: " + QString::number(data.size()));
    file.close();
}

QStringList TestDataset::labels() const {
    return _labels;
}

double TestDataset::value(int row, int col) const {
    Q_ASSERT(row < rows && col < columns);
    return data.at(row*columns + col);
}


void TestDataset::openFile(QString fileName) {
    filePath = FileLocations::location(FileLocationInfo::Output).absolutePath() +
                       "/" + fileName;
    file.setFileName(filePath);

    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk)
        throw Exception("Cannot open output file: " + filePath);
}

void TestDataset::readLineItems() {
    QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine()).trimmed();
    }
    lineItems = line.split("\t", QString::SkipEmptyParts);
}

void TestDataset::readData() {
    int numItems = -1;
    while (true) {
        readLineItems();
        if (lineItems.isEmpty()) break;
        if (numItems == -1) {
            numItems = lineItems.size();
            _labels = lineItems;
        }
        else {
            if (lineItems.size() != _labels.size())
                throw Exception("Number of columns does not match in file: " + filePath);
            for (int i = 0; i < numItems; ++i) {
                bool ok;
                data.append(lineItems[i].toDouble(&ok));
                if (!ok)
                    throw Exception("Item is not a number: '" + lineItems[i] + "'' in file: " + filePath);
            }
        }
    }
}


} //namespace

