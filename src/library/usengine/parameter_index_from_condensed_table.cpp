/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include <usbase/model.h>
#include "parameter_index_from_condensed_table.h"
namespace UniSim{

ParameterIndexFromCondensedTable::ParameterIndexFromCondensedTable(QString filePath, QString parameterName_)
    : ParameterIndex(filePath), parameterName(parameterName_)
{
    if (table->numKeys() != 1) {
        QString msg = "Table in file '%1' must hold one column marked as a key with '*'";
        throw Exception(msg.arg(filePath));
    }
}

void ParameterIndexFromCondensedTable::reset(Model *model) {
    DataGrid::KeySubset keys;
    Identifier id = Identifier( table->columnNames().value(0) );
    keys[0] = model->peekKeyValue(id);
    QVector<int> rowIndices = QVector<int>::fromList( table->rowIndices(keys) );
    if (rowIndices.size() == 0) {
        _hasNext = false;
    }
    else if (rowIndices.size() == 1) {
        nextRowIndex = rowIndices.at(0);
        QString modelName = model->id().label();
        nextColumnIndex = table->columnNames().indexOf(modelName);
        if (nextColumnIndex == -1) {
            QString msg = "No column mathes model named '%1' in file '%2'";
            throw Exception(msg.arg(modelName).arg(filePath), model);
        }
        _hasNext = true;
    }
    else {
        QString msg = "No duplicate keys allowed. Error in file '%1'";
        throw Exception(msg.arg(filePath), model);
    }
}

bool ParameterIndexFromCondensedTable::hasNext() {
    return _hasNext;
}

ParameterIndex::Result ParameterIndexFromCondensedTable::next() {
    Q_ASSERT(hasNext());
    QString value = table->row(nextRowIndex).at(nextColumnIndex);
    _hasNext = false;
    return qMakePair(parameterName, value);
}

} //namespace

