/* Copyright (C) 2009-2011 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include <usbase/data_grid.h>
#include <usbase/exception.h>
#include "parameter_index.h"

namespace UniSim{

ParameterIndex::ParameterIndex(QString filePath_) {
    filePath = filePath_;
    table = new DataGrid(filePath);
    if (table->numKeys() == 0) {
        QString msg = "Table in file '%1' must hold at least one column marked as a key with '*'";
        throw Exception(msg.arg(filePath));
    }
}

ParameterIndex::~ParameterIndex() {
    delete table;
}


} //namespace
