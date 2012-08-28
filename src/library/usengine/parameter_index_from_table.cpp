/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include <usbase/model.h>
#include "parameter_index_from_table.h"

namespace UniSim{

ParameterIndexFromTable::ParameterIndexFromTable(QString filePath)
    : ParameterIndex(filePath)
{
    if (table->numKeys() == 0) {
        QString msg = "Table in file '%1' must hold at least one column marked as a key with '*'";
        throw Exception(msg.arg(filePath));
    }
}

void ParameterIndexFromTable::reset(Model *model) {
    DataGrid::KeySubset keys;
    for (int i = 0; i < table->numKeys(); ++i) {
        Identifier id = Identifier( table->columnNames().value(i) );
        keys[i] = model->peekKeyValue(id);
    }
    QVector<int> rowIndices = QVector<int>::fromList( table->rowIndices(keys) );
    if (rowIndices.size() == 0) {
        nextColumnIndex = table->columnNames().size();  // nothing is next
    }
    else if (rowIndices.size() == 1) {
        nextRowIndex = rowIndices.at(0);
        nextColumnIndex = table->numKeys();
    }
    else {
        QString msg = "No duplicate keys allowed. Error in file '%1'";
        throw Exception(msg.arg(filePath), model);
    }
}

bool ParameterIndexFromTable::hasNext() {
    return nextColumnIndex < table->columnNames().size();
}

ParameterIndex::Result ParameterIndexFromTable::next() {
    Q_ASSERT(hasNext());
    QString paramName = table->columnNames().at(nextColumnIndex);
    QString value = table->row(nextRowIndex).at(nextColumnIndex);
    ++nextColumnIndex;
    return qMakePair(paramName, value);
}

} //namespace

