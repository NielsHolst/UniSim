/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <QFile>
#include "exception.h"
#include "data_grid.h"
#include "utilities.h"

namespace UniSim{

DataGrid::DataGrid(QString _filePath, QObject *parent)
    : QObject(parent), filePath(_filePath)
{
    readData();
}

int DataGrid::numKeys() const {
    return data.numKeys;
}

int DataGrid::rowNumber() const {
    return data.rows.size();
}

int DataGrid::columnNumber() const {
    return data.columnNamesInOrder.size();
}

QStringList DataGrid::columnNames() const {
    return data.columnNamesInOrder;
}

QStringList DataGrid::rowNames() const {
    // return QStringList( data.rowIndex.keys() );
    return data.rowKeysInOrder;
}

QStringList DataGrid::row(int row) const {
    Q_ASSERT(0 <= row && row < data.rows.size());
    return data.rows.value(row);
}

QStringList DataGrid::row(const QStringList &rowKeys) const {
    QString rowKey = joinKeys(rowKeys);
    if (!data.rowIndex.contains(rowKey)) {
        QString msg = "No row with key value '%1' exists in file '%2'";
        throw Exception(msg.arg(rowKey).arg(filePath));
    }
    int row = data.rowIndex.value(rowKey);
    return data.rows.value(row);
}

QList<int> DataGrid::rowIndices(const DataGrid::KeySubset &rowKeys) const {
    /*
    QByteArray key = keyToSubIndex(rowKeys.keys());
    if (!subIndices.contains(key))
        const_cast<DataGrid*>(this)->createSubIndex(rowKeys.keys());
    Index *subIndex = subIndices.value(key);
    return subIndex->values( joinKeys(rowKeys.values()) );
    */
    QList<int> indices;
    int nrow = data.rows.size();
    for (int row = 0; row < nrow; ++row) {
        QStringList aRow = data.rows.value(row);
        bool ok(true);
        KeySubsetIterator i(rowKeys);
        while (ok && i.hasNext()) {
            i.next();
            ok = aRow.value(i.key()) == i.value();
        }
        if (ok)
            indices << row;
    }
    return indices;
}

QByteArray DataGrid::keyToSubIndex(const QList<int> &keyColumns) const {
    QByteArray subIndex;
    for (int i = 0; i < keyColumns.size(); ++i) {
        subIndex.append( char(keyColumns.value(i)) );
    }
    return subIndex;
}

void DataGrid::createSubIndex(const QList<int> &keyColumns) {
    Index *subIndex = new Index;
    //subIndices[ keyToSubIndex(keyColumns) ] = subIndex;

    QStringList keys;
    int nrow = data.rows.size();
    int nkey = keyColumns.size();
    for (int row = 0; row < nrow; ++row) {
        QStringList aRow = data.rows.value(row);
        for (int k = 0; k < nkey; ++k) {
            int col = keyColumns.value(k);
            QString keyValue = aRow.value(col);
            keys << keyValue;
        }
        (*subIndex)[joinKeys(keys)] = row;
    }
}

QString DataGrid::cell(int _row, int col) const {
    Q_ASSERT(0 <= col && col < data.columnIndex.size());
    return row(_row).value(col);
}

QString DataGrid::cell(const QStringList &rowKeys, QString colKey) const {
    if (!data.columnIndex.contains(colKey)) {
        QString msg("No column with key value '%1' exists in file '%2'");
        throw Exception(msg.arg(colKey).arg(filePath));
    }
    int col = data.columnIndex.value(colKey);
    return row(rowKeys).value(col);
}

QString DataGrid::joinKeys(QStringList keys) const {
    return keys.join("-");
}

void DataGrid::readData() {
    readFirstLine();
    setColumnIndex();
    while (true) {
        readLineItems();
        if (pastLastLine) break;
        checkLine();
        appendLine();
    }
    file.close();
}

void DataGrid::readFirstLine() {
    file.setFileName(filePath);
    bool fileOk = file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!fileOk) {
        QString msg("Cannot open DataGrid file '%1'");
        throw Exception(msg.arg(filePath));
    }
    readLineItems();
    if (pastLastLine) {
        QString msg("DataGrid file is empty: '%1'");
        throw Exception(msg.arg(filePath));
    }
}

void DataGrid::readLineItems() {
    QString line;
    while (!file.atEnd() && line.isEmpty()) {
        line = QString(file.readLine().trimmed());
    }
    lineItems = line.split("\t", QString::SkipEmptyParts);
    pastLastLine = lineItems.isEmpty();
}

void DataGrid::setColumnIndex() {
    data.numKeys = 0;
    for (int i = 0; i < lineItems.size(); ++i) {
        QString name = lineItems.value(i);
        if (name.startsWith("*")) {
            if (i != data.numKeys) {
                QString msg("Key columns (marked with '*') must be the leftmost columns in the file. "
                            "Fix column '%1' in file '%2'");
                throw Exception(msg.arg(name).arg(filePath));
            }
            ++data.numKeys;
            name.remove(0,1);
        }
        if (data.columnIndex.contains(name)) {
            QString msg("Column names must be unique: '%1' occurs twice in file '%2'.\nLine: '%3'");
            throw Exception(msg.arg(name).arg(filePath).arg(lineItems.join(" ")));
        }
        data.columnIndex[name] = i;
        data.columnNamesInOrder << name;
    }
}

void DataGrid::checkLine() {
    int n1 = data.columnIndex.size(), n2 = lineItems.size();
    if (n1 != n2) {
        int rowNumber = data.rowIndex.size() + 2;
        QString msg("Error in DataGrid file '%1', row number %2; "
                    "The number of columns labels does not match the number of values in row (%3 vs. %4)");
        throw Exception(msg.arg(filePath).arg(rowNumber).arg(n1).arg(n2));
    }
}

void DataGrid::appendLine() {
    data.rows.append(lineItems);
    if (data.numKeys == 0)
        return;

    QStringList rowKeys( lineItems.mid(0, data.numKeys) );
    QString rowKey = joinKeys(rowKeys);
    if (data.rowIndex.contains(rowKey)) {
        int rowNumber = data.rowIndex.size() + 2;
        QString msg("Error in DataGrid file '%1', row number %2; "
                    "Row keys must be unique: '%3' occurs twice");
        throw Exception(msg.arg(filePath).arg(rowNumber).arg(rowKey));
    }
    data.rowIndex[rowKey] = data.rows.size() - 1;
    data.rowKeysInOrder << rowKey;
}

} //namespace


