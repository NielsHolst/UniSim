/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include <usbase/model.h>
#include <usbase/parameter_base.h>
#include "instance_index_from_table.h"

namespace UniSim{

InstanceIndexFromTable::InstanceIndexFromTable(QString filePath)
    : InstanceIndex()
{
    table = new DataGrid(filePath);
    if (table->numKeys() == 0) {
        QString msg = "Table in file '%1' must hold at least one column marked as a key with '*'";
        throw Exception(msg.arg(filePath));
    }
}

void InstanceIndexFromTable::reset(QString modelType, Model *parent) {
    if (table->numKeys() == 1) {
        useAllRows();
        columnIndexOfModelType = 0;
        return;
    }

    Q_ASSERT(parent);
    DataGrid::KeySubset keys;
    setColumnIndexOfModelType(modelType);
    int excludeKey = columnIndexOfModelType;
    for (int i = 0; i < table->numKeys(); ++i) {
        if (i==excludeKey) continue;
        Identifier id = Identifier( table->columnNames().value(i) );
        keys[i] = parent->peekKeyValue(id);
    }
    indices = QVector<int>::fromList( table->rowIndices(keys) );
    nextIndex = 0;
}

void InstanceIndexFromTable::useAllRows() {
    indices.clear();
    int numIndices = table->rowNames().size();
    for (int i = 0; i < numIndices; ++i) {
        indices << i;
    }
    nextIndex = 0;
}


void InstanceIndexFromTable::setColumnIndexOfModelType(QString modelType) {
    int col = table->columnNames().indexOf(modelType);
    if (col == -1) {
        QString msg("Table needs a column heading: '%1'");
        throw Exception(msg.arg("*"+modelType));
    }
    columnIndexOfModelType = col;
}

bool InstanceIndexFromTable::hasNext() {
    return nextIndex < indices.size();
}

InstanceIndex::Record InstanceIndexFromTable::next() {
    Q_ASSERT(hasNext());
    int rowIndex = indices.at(nextIndex);
    InstanceIndex::Record rec;
    rec.modelName = table->cell(rowIndex, columnIndexOfModelType);

    QStringList columnValues = table->row(rowIndex);
    int numColumns = columnValues.size();
    for (int i = table->numKeys(); i < numColumns; ++i) {
        QString columnName = table->columnNames().at(i);
        QString value = columnValues.at(i);
        rec.paramNameValue[columnName] = value;
    }
    ++nextIndex;
    return rec;
}




} //namespace

