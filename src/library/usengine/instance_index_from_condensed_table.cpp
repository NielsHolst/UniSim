/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include <usbase/model.h>
#include <usbase/parameter_base.h>
#include "instance_index_from_condensed_table.h"

namespace UniSim{

InstanceIndexFromCondensedTable::InstanceIndexFromCondensedTable(QString filePath_)
    : InstanceIndex()
{
    filePath = filePath_;
    table = new DataGrid(filePath);
    if (table->numKeys() != 1) {
        QString msg = "Table in file '%1' must hold one and only one column marked as a key with '*'";
        throw Exception(msg.arg(filePath));
    }
}

void InstanceIndexFromCondensedTable::reset(QString parameterName_, Model *parent) {
    Q_ASSERT(parent);
    parameterName = parameterName_;
    DataGrid::KeySubset keys;
    Identifier id = Identifier( table->columnNames().value(0) );
    keys[0] = parent->peekKeyValue(id);
    QVector<int> rowIndices = QVector<int>::fromList( table->rowIndices(keys) );
    if (rowIndices.size() == 0) {
        nextColumnIndex = table->columnNames().size();  // nothing is next
    }
    else if (rowIndices.size() == 1) {
        nextRowIndex = rowIndices.at(0);
        nextColumnIndex = 1;
    }
    else {
        QString msg = "No duplicate keys allowed. Error on key '%1' in file '%2'";
        throw Exception(msg.arg(id.label().arg(filePath)), parent);
    }
}

bool InstanceIndexFromCondensedTable::hasNext() {
    return nextColumnIndex < table->columnNames().size();
}

InstanceIndex::Record InstanceIndexFromCondensedTable::next() {
    Q_ASSERT(hasNext());
    InstanceIndex::Record rec;

    rec.modelName = table->columnNames().at(nextColumnIndex);
    QString value = table->row(nextRowIndex).at(nextColumnIndex);
    rec.paramNameValue[parameterName] = value;

    ++nextColumnIndex;
    return rec;
}




} //namespace

