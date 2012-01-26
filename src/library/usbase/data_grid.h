/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef UNISIM_DATA_GRID_H
#define UNISIM_DATA_GRID_H

#include <QFile>
#include <QHash>
#include <QList>
#include <QMap>
#include <QPair>
#include <QStringList>
#include <QVector>

namespace UniSim{

class DataGrid
{
public:
    DataGrid(QString filePath);

    typedef QMapIterator<int,QString> KeySubsetIterator; // <column number, key value>
    typedef QMap<int,QString> KeySubset; // <column number, key value>

    int numKeys() const;
    int rowNumber() const;
    int columnNumber() const;
    QStringList rowNames() const;
    QStringList columnNames() const;
    QStringList row(int row) const;
    QStringList row(const QStringList &rowKeys) const;
    QList<int> rowIndices(const KeySubset &rowKeys) const;
    QString cell(int row, int col) const;
    QString cell(const QStringList &rowKeys, QString colKey) const;

private:
    // grid data
    typedef QHash<QString, int> Index;
    struct {
        QVector<QStringList> rows;
        Index rowIndex, columnIndex;
        QStringList columnNamesInOrder;
        int numKeys;
    } data;
    //QHash<QByteArray, Index*> subIndices;

    // methods
    QByteArray keyToSubIndex(const QList<int> &keyColumns) const;
    void createSubIndex(const QList<int> &keyColumns);
    QString joinKeys(QStringList keys) const;
    void readData();
    void readFirstLine();
    void readLineItems();
    void setColumnIndex();
    void checkLine();
    void appendLine();

    // housekeeping data
    QString filePath;
    QFile file;
    QStringList lineItems;
    bool pastLastLine;
};

} //namespace

#endif

